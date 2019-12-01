#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mss9cc.h"

LVar *function_set_s = NULL;
LVar *function_set_e = NULL;

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
	int val = token->val;
	token = token->next;
	return val;
}

bool at_eof() {
	return token->kind == TK_EOF;
}

LVar *find_lvar(Token *tok, Node *node){
	
	LVar *locals_ss;

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
void make_lvar(Token *tok, Node *node, int param_f, Ty ty){
	LVar *lvar = calloc(1, sizeof(LVar));
	lvar->len = tok->len;
	lvar->name = tok->str;
	lvar->defnode = node;
	if(cur_node->locals_e){
		cur_node->locals_e->next = lvar;
		lvar->offset = cur_node->var_size + 8;
		if(param_f)	cur_node->params_cnt++;
		else 	cur_node->locals_cnt++;
	}else{
		cur_node->locals_s = lvar;
		lvar->offset = 8;
		if(param_f)cur_node->params_cnt = 1;
		else	cur_node->locals_cnt = 1;
	}
	/* スタック上の変数領域の範囲を更新 */
	if(ty == INT)	cur_node->var_size += 8;
	else if(ty == PTR)	cur_node->var_size	+= 8;
	else if(ty == ARRAY)	cur_node->var_size += 8*node->type->array_size;	
	/* ===== */
	if(node)	node->offset = lvar->offset;
	cur_node->locals_e = lvar;
	return;
}

Node *find_tree_type(Node *node){
	if(node->kind == ND_LVAR || node->kind == ND_APP || node->kind == ND_DEREF
		|| node->kind == ND_ADDR)	return node;
	Node *lhs, *rhs;
	if(node->lhs)	lhs = find_tree_type(node->lhs);
	if(node->rhs)	rhs = find_tree_type(node->rhs);
	if(!lhs && !rhs)	return NULL;
	else if(lhs)	return lhs;
	else if(rhs)	return rhs;
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
		if(node->kind == ND_FUN) return node;
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
	}else if(consume("&")){
		return new_node(ND_ADDR, unary(), NULL);
	}else if(consume("*")){
		Node *node = calloc(1, sizeof(Node));
		node->kind = ND_DEREF;
		node->type = calloc(1, sizeof(Type));
		node->type->ty = PTR;
		Type *type = node->type;
		for(;consume("*");type = type->ptr_to){
			type->ptr_to = calloc(1, sizeof(Type));
			type->ptr_to->ty = PTR;
		}
		node->lhs = unary();
		type->ptr_to = node->lhs->type;
		return node;
	}else if(consume("sizeof")){
		Node *rhs = unary();
		Node *typenode = find_tree_type(rhs);
		// typenode == NULL はND_NUMを表す
		if(!typenode || typenode->kind == ND_ADDR || typenode->kind == ND_DEREF)	return new_node_num(8);
		else if(typenode->kind == ND_LVAR || typenode->kind == ND_APP){
			if(!typenode->type)	error_at(token->str, "パーズで変数に型がありません.");
			else if(typenode->type->ty == INT)	return new_node_num(8);
			else if(typenode->type->ty == PTR)	return new_node_num(8);
			else if(typenode->type->ty == ARRAY)	return new_node_num(rhs->type->array_size*8);
		}else{
			error_at(token->str, "構文木の型がありません.");
		}
	}
	else{
		return primary();
	}
}

Node *primary(){

	if(consume("(")){
		Node *node = expr();
		expect(')');
		return node;
	}
	int def_flag = 0;
	Node *par = calloc(1, sizeof(Node));
	Type *this_type = calloc(1, sizeof(Type));
	// 型定義があるかの判定
	if(token->len == 3 && strncmp(token->str, "int", token->len) == 0){
		def_flag = 1;
		token = token->next;
		Type *type = this_type;
		for(;consume("*");type = type->ptr_to){
			type->ty = PTR;
			type->ptr_to = calloc(1, sizeof(Type));
		}
		type->ty = INT;
		if(this_type->ty == PTR){
			par->kind = ND_DEREF;
			par->type = this_type;
		}
	}
	// =====
	Token *tok = consume_ident();
	if(tok){
		Node *node = calloc(1, sizeof(Node));
		/* ポインタ定義の場合は親ノードを付ける */
		if(par->kind){
			par->lhs = calloc(1, sizeof(Node));
			node->par = par;
		}
		/* ノードの型を決める */
		node->type = calloc(1, sizeof(Type));
		node->type->ty = INT;
		/* ===== */
		Token *token2 = token;
		token = token->next;
		/* 関数 */
		if(consume("(")){
			/* 関数名の保存*/
			strncpy(node->token, token2->str, token2->len);
			*(node->token+token2->len) = '\0';
			/* 関数の定義または呼び出しの判断 */
			token2 = token;
			for(;!consume_moveon(")");){
				if(!token->next)	error_at(token->str, "consume_moveonで不正なtokenが検出されました.");
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
				if(!def_flag)	error_at(token->str, "関数の型が定義されていません.");
				if(!consume(")")){
					while(1){
						if(strncmp(token->str, "int", token->len) == 0){
							token = token->next;
						}else{
							error_at(token->str, "関数定義の引数の型が不正です.");
						}
						Token *tok = consume_ident();
						if(!tok)	error("関数定義の仮引数が正しくありません.");
						make_lvar(tok, NULL, 1, node->type->ty);
						token = token->next;
						if(!consume(")"))	expect(',');
						else 	break;
					}
				}
				// 関数定義の名前の追加
				LVar *funlvar = calloc(1, sizeof(LVar));
				funlvar->name = tok->str;
				funlvar->len = tok->len;
				if(!function_set_s){
					function_set_s = function_set_e = funlvar;
				}else{
					function_set_e->next = funlvar;
					function_set_e = funlvar;
				}
				/* ブロック内の処理 */
				node->lhs = stmt();
				return node;
			/* 関数適用の場合 */
			}else if(node->kind == ND_APP){
				if(def_flag)	error_at(token->str, "関数適用に型は必要ありません.");
				// 関数が存在するか確認
				LVar *lvar = function_set_s;
				for(;lvar;lvar = lvar->next){
					if(lvar->len == tok->len && strncmp(lvar->name, tok->str, tok->len) == 0) break;
				}
				if(!lvar)	error_at(token->str, "定義されていない関数の参照です.");
				// =====
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
				return node;
			/* ===== */
			}else{
				error_at(token->str, "関数の定義もしくは呼び出しのパースで不正なノードを検出しました.");
			}
		/* 配列 */
		}else if(consume("[")){
			if(def_flag){
				int size = expect_number();
				expect(']');
				par->kind == ND_DEREF;
				par->type = calloc(1, sizeof(Type));
				par->type->ty = PTR;
				par->type->ptr_to = this_type;
				node->type->array_size = size;
				node->type->ty = ARRAY;
				node->kind = ND_LVAR;
				par->lhs = node;
				make_lvar(tok, node, 0, ARRAY);
				return node;
			}
		/* 変数 */
		}else{
			node->kind = ND_LVAR;
		}
		LVar *lvar;
		if(!cur_node)	error_at(token->str, "NULLに対してmake_lvarを呼び出そうとしています.");
		else 	lvar = find_lvar(tok, cur_node);
		if(lvar){
			node->offset = lvar->offset;
			node->defnode = lvar->defnode;
			if(lvar->defnode)	node->type = lvar->defnode->type;	// 関数定義の引数にはdefnodeは存在しない
		}else if(def_flag){
			make_lvar(tok, node, 0, node->type->ty);
		}else{
			error_at(token->str, "定義されていない変数の参照です.");
		}
		return node;
	}else if(def_flag){
		error_at(token->str, "変数以外に型はつけられません.");
	}
	
	return new_node_num(expect_number());
}