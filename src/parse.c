#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mss9cc.h"

// LVar *locals_s; // list of variables
// LVar *locals_e;

// 次のトークンが期待している記号のときには、トークンを1つ読み進めて
// 真を返す。それ以外の場合には偽を返す。
bool consume(char *op) {
	// if (token->kind != TK_RESERVED || token->str[0] != op)
	if(token->len != strlen(op) || memcmp(token->str, op, token->len)){
		return false;
	}
	token = token->next;
	return true;
}

bool consume_tokenstay(char *op){
	if(token->len != strlen(op) || memcmp(token->str, op, token->len)){
		return false;
	}
	return true;
}

bool consume_moveon(char *op){
	
	if(token->len != strlen(op) || memcmp(token->str, op, token->len)){
		token = token->next;
		return false;
	}
	token = token->next;
	return true;
}

Token *consume_ident(){
	
	if(token->kind != TK_IDENT || token->str[0] > 'z' || token->str[0] < 'a'){
		return NULL;
	}
	return token;
}

// 次のトークンが期待している記号のときには、トークンを1つ読み進める。
// それ以外の場合にはエラーを報告する。
void expect(char op) {
	if (token->kind != TK_RESERVED || token->str[0] != op)
	error_at(token->str, "'%c'ではありません", op);
	token = token->next;
}

// 次のトークンが数値の場合、トークンを1つ読み進めてその数値を返す。
// それ以外の場合にはエラーを報告する。
int expect_number() {
	if (token->kind != TK_NUM)
	error_at(token->str, "数ではありません");
	// error(token->str);
	int val = token->val;
	token = token->next;
	return val;
}

bool at_eof() {
	return token->kind == TK_EOF;
}

LVar *find_lvar(Token *tok, Node *node){
	
	LVar *locals_ss;

	if(!node)	error("find_lvarがNULLノードに対して呼ばれました.");
	locals_ss = node->locals_s;
	for(LVar *lvar = locals_ss;lvar;lvar = lvar->next){
		if(lvar->len == tok->len && memcmp(tok->str, lvar->name, lvar->len) == 0){
			return lvar;
		}
	}
	return NULL;
}

/*
	make parser tree
*/

Node *new_node(Nodekind kind, Node *lhs, Node *rhs){
	Node *node = calloc(1, sizeof(Node));
	node->kind = kind;
	node->lhs = lhs;
	node->rhs = rhs;
	return node;
}

Node *new_node_num(int val){
	Node *node = calloc(1, sizeof(Node));
	node->kind = ND_NUM;
	node->val = val;
	return node;
}

/* 現在対象のノードにLVarを追加する */
void make_lvar(Token *tok, Node *node, int param_f){
	LVar *lvar = calloc(1, sizeof(LVar));
	lvar->len = tok->len;
	lvar->name = tok->str;
	if(cur_node->locals_e){
		cur_node->locals_e->next = lvar;
		lvar->offset = cur_node->locals_e->offset + 8;
		if(param_f)	cur_node->params_cnt++;
		else 	cur_node->locals_cnt++;
	}else{
		cur_node->locals_s = lvar;
		lvar->offset = 8;
		if(param_f)	cur_node->params_cnt = 1;
		else 	cur_node->locals_cnt = 1;
	}
	if(node)	node->offset = lvar->offset;
	cur_node->locals_e = lvar;
	return;
}

char* nodekind2str(Nodekind kind){
	if(ND_ADD)	return "+";
	else if(ND_SUB)	return "-";
	else if(ND_MUL)	return "*";
	else if(ND_DIV)	return "/";
	else if(ND_NUM)	return "digit";
	else if(ND_EQU) return "==";
	else if(ND_NOTEQU) return "!=";
	else if(ND_RIGHTINE) return "<";
	else if(ND_RINEEQU) return "<=";
	else if(ND_LEFTINE) return ">";
	else if(ND_LINEEQU)	return ">=";
	else if(ND_ASSIGN)	return "=";
	else if(ND_LVAR)	return "locvar";
	else if(ND_SEMICORO)	return ";";
	else if(ND_RETURN)	return "return";
	else if(ND_IF)	return "if";
	else if(ND_ELSE)	return "else";
	else if(ND_WHILE)	return "while";
	else if(ND_FOR)		return "for";
	else if(ND_FOREXPR)	return "forexpr";
	else if(ND_BLOCK)	return "{}";
	else if(ND_APP)	return "fun ->";
	else return "nodekind2str error";
}

char *syntax_debug(Node *code){
	
	char *str[4];
	char *stra = calloc(1, sizeof(char*));
	Nodekind kind = code->kind;

	for(int i = 0;i < 4;i++)	str[i] = "NULL";
	if(code->lhs)	str[0] = syntax_debug(code->lhs);
	if(code->mhs)	str[1] = syntax_debug(code->mhs);
	if(code->lhs)	str[2] = syntax_debug(code->lhs);
	if(code->vector){
		str[3] = "(";
		while(1){
			code = code->vector;
			str[3] = strcat(str[3], syntax_debug(code));
			if(code->vector)	strcat(str[3], ", ");
			else{
				strcat(str[3], ")");
				break;
			}
		}
	}
	sprintf(stra, "(%s, %s, %s, %s, %s)", nodekind2str(kind), str[0], str[1], str[2], str[3]);
	return stra;
}


/*
	exec parse
*/

Node *code[100];

void program(){
	
	int i = 0;
	while(!at_eof()){
		code[i++] = stmt();
	}
	code[i] = NULL;

}

Node *stmt(){

	Node *node;
	
	if(consume("return")){
		node = calloc(1, sizeof(Node));
		node->kind = ND_RETURN;
		node->lhs = expr();
	}else if(consume("if")){
		node = calloc(1, sizeof(Node));
		node->kind = ND_IF;
		expect('(');
		node->lhs = expr();		// ifの条件式
		expect(')');
		node->mhs = stmt();		// ifが真の場合の処理
		if(consume_tokenstay("else")){
			if(consume("else") && consume_tokenstay("if")){
				node->rhs = stmt();
			}else{
				Node *elsenode = calloc(1, sizeof(Node));
				elsenode->kind = ND_ELSE;
				elsenode->rhs = stmt();
				node->rhs = elsenode;
			}
		}
		return node;
	}else if(consume("while")){
		node = calloc(1, sizeof(Node));
		node->kind = ND_WHILE;
		expect('(');
		node->lhs = expr();
		expect(')');
		node->rhs = stmt();
		return node;
	}else if(consume("for")){
		node = calloc(1, sizeof(Node));
		Node *node_cond = calloc(1, sizeof(Node));
		node_cond->kind = ND_FOREXPR;
		node->kind = ND_FOR;
		expect('(');
		if(!consume(";")){
			node_cond->lhs = expr();
			expect(';');
		}
		if(!consume(";")){
			node_cond->mhs = expr();
			expect(';');
		}
		if(!consume(")")){
			node_cond->rhs = expr();
			expect(')');
		}
		node->lhs = node_cond;
		node->rhs = stmt();
		return node;
	}else if(consume("{")){
		node = calloc(1, sizeof(Node));
		node->kind = ND_BLOCK;
		Node *vec = node;
		while(!consume("}")){
			vec->vector = stmt();
			vec = vec->vector;
		}
		return node;
	}else{
		node = expr();
		if(node->kind == ND_FUN)	return node;
	}
	expect(';');
	return node;

}

Node *expr(){
	return assign();
}

Node *assign(){

	Node *node = equarity();
	if(consume("=")){
		node = new_node(ND_ASSIGN, node, equarity());
	}
	return node;

}

Node *equarity(){

	Node *node = relational();

	for(;;){
		if(consume("==")){
			node = new_node(ND_EQU, node, relational());
		}else if(consume("!=")){
			node = new_node(ND_NOTEQU, node, relational());
		}else{
			return node;
		}
	}
}

Node *relational(){

	Node * node = add();

	for(;;){
		if(consume("<")){
			node = new_node(ND_RIGHTINE, node, add());
		}else if(consume("<=")){
			node = new_node(ND_RINEEQU, node, add());
		}else if(consume(">")){
			node = new_node(ND_LEFTINE, node, add());
		}else if(consume(">=")){
			node = new_node(ND_LINEEQU, node, add());
		}else{
			return node;
		}
	}
}

Node *add(){

	Node *node = mul();

	for(;;){
		if(consume("+")){
			node = new_node(ND_ADD, node, mul());
		}else if(consume("-")){
			node = new_node(ND_SUB, node, mul());
		}else{
			return node;
		}
	}
}

Node *mul(){

	Node *node = unary();

	for(;;){
		if(consume("*")){
			node = new_node(ND_MUL, node, unary());
		}else if(consume("/")){
			node = new_node(ND_DIV, node, unary());
		}else{
			return node;
		}
	}
}

Node *unary(){
	if(consume("+")){
		return primary();
	}else if(consume("-")){
		return new_node(ND_SUB, new_node_num(0), primary());
	}else{
		return primary();
	}
}

Node *primary(){

	if(consume("(")){
		Node *node = expr();
		expect(')');
		return node;
	}
	Token *tok = consume_ident();
	if(tok){
		Node *node = calloc(1, sizeof(Node));
		Token *token2 = token;
		token = token->next;
		if(consume("(")){
			/* 関数名の保存*/
			strncpy(node->token, token2->str, token2->len);
			*(node->token+token2->len) = '\0';
			/* 関数の定義または呼び出しの判断 */
			token2 = token;
			for(;!consume_moveon(")");){
				if(!token->next)	error("consume_moveonで不正なtokenが検出されました.");
				continue;
			}
			if(consume("{")){
				node->kind = ND_FUN;
				cur_node = node;
			}else{
				node->kind = ND_APP;
			}
			token = token2;
			/* === */
			Node *vec = node;
			/* 関数定義の場合 */
			if(node->kind == ND_FUN){
				if(!consume(")")){
					while(1){
						Token *tok = consume_ident();
						if(!tok)	error("関数定義の仮引数が正しくありません.");
						make_lvar(tok, NULL, 1);
						token = token->next;
						if(!consume(")"))	expect(',');
						else 	break;
					}
				}
				/* ブロック内の処理 */
				node->lhs = stmt();
			/* 関数適用の場合 */
			}else if(node->kind == ND_APP){
				node->params_cnt = 0;
				if(!consume(")")){
					while(1){
						vec->params = expr();
						vec = vec->params;
						node->params_cnt++;
						if(!consume(",")){
							expect(')');
							break;
						}
					}
				}
			}else{
				error("関数の定義もしくは呼び出しのパースで不正なノードを検出しました.");
			}
		}else{
			node->kind = ND_LVAR;
		}
		LVar *lvar;
		if(!cur_node)	lvar = find_lvar(tok, NULL);
		else 	lvar = find_lvar(tok, cur_node);
		if(lvar){
			node->offset = lvar->offset;
		}else{
			make_lvar(tok, node, 0);
		}
		return node;
	}
	return new_node_num(expect_number());
}