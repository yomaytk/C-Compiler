#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mss9cc.h"

LVar *function_set_s;
LVar *function_set_e;

LVar *globals_s;
LVar *globals_e;

String *string_s;
String *string_e;

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

Node *new_node(Ty ty, Nodekind kind, Node *lhs, Node *rhs){
	Node *node = calloc(1, sizeof(Node));
	node->kind = kind;
	node->lhs = lhs;
	node->rhs = rhs;
	node->type = calloc(1, sizeof(Type));
	node->type->ty = ty;
	return node;
}

Node *new_node_num(int val){
	Node *node = calloc(1, sizeof(Node));
	node->kind = ND_NUM;
	node->val = val;
	node->type = calloc(1, sizeof(Type));
	node->type->ty = INT;
	return node;
}

/* 現在対象の環境にLVarを追加 */
void add_lvar(Token *tok, Node *node, int param_f, Ty ty){
	LVar *lvar = calloc(1, sizeof(LVar));
	lvar->len = tok->len;
	lvar->name = tok->str;
	lvar->defnode = node;
	if(!node)	lvar->params_ty = ty;	// 関数定義の仮引数の型を保存
	// 変数が一つ目かそうでないか
	if(cur_node->locals_e)	cur_node->locals_e->next = lvar;
	else	cur_node->locals_s = lvar;
	// 仮引数の変数かどうか
	if(param_f)	cur_node->params_cnt++;
	else 	cur_node->locals_cnt++;
	// スタック上の変数領域の範囲を更新
	if(ty == ARRAY_CHAR)	cur_node->var_size += 1*node->type->array_size;
	else if(ty == ARRAY_INT)	cur_node->var_size += 4*node->type->array_size;
	else if(node && node->par)	cur_node->var_size += 8;		// 関数定義の時はnode == NULL
	else if(ty == INT)	cur_node->var_size += 4;
	else if(ty == CHAR)	cur_node->var_size += 1;
	// =====
	lvar->offset = cur_node->var_size;
	if(node)	node->offset = lvar->offset;
	cur_node->locals_e = lvar;
	return;
}

/* グローバル環境にLVarを追加 */
void add_gblvar(Token *tok, Node *node){
	LVar *lvar = calloc(1, sizeof(LVar));
	lvar->len = tok->len;
	lvar->name = tok->str;
	lvar->defnode = node;
	// 変数が一つ目かそうでないか
	if(globals_s)	globals_e->next = lvar;
	else 	globals_s = lvar;
	// =====
	globals_e = lvar;
	return;
}

/* 現在対象の環境から対象の変数を返す */
LVar *find_lvar(Token *tok, Node *node){
	
	LVar *locals_ss = node->locals_s;			// 修正の余地

	for(LVar *lvar = locals_ss;lvar;lvar = lvar->next){
		if(lvar->len == tok->len && memcmp(tok->str, lvar->name, lvar->len) == 0){
			return lvar;
		}
	}
	return NULL;
}

/* グローバル環境から対象の変数を返す */
LVar *find_gblvar(Token *tok){
	
	for(LVar *gblvar = globals_s;gblvar;gblvar = gblvar->next){
		if(gblvar->len == tok->len && memcmp(tok->str, gblvar->name, gblvar->len) == 0){
			return gblvar;
		}
	}
	return NULL;
}


Node *find_tree_type(Node *node){
	if(node->kind == ND_LVAR || node->kind == ND_APP || node->kind == ND_DEREF
		|| node->kind == ND_ADDR || node->kind == ND_GBLVAR)	return node;
	Node *lhs, *rhs;
	if(node->lhs)	lhs = find_tree_type(node->lhs);
	if(node->rhs)	rhs = find_tree_type(node->rhs);
	if(!lhs && !rhs)	return NULL;
	else if(lhs)	return lhs;
	else if(rhs)	return rhs;
}

char* nodekind2str(Nodekind kind){
	if(kind == ND_ADD)	return "+";
	else if(kind == ND_SUB)	return "-";
	else if(kind == ND_MUL)	return "*";
	else if(kind == ND_DIV)	return "/";
	else if(kind == ND_NUM)	return "digit";
	else if(kind == ND_EQU) return "==";
	else if(kind == ND_NOTEQU) return "!=";
	else if(kind == ND_RIGHTINE) return "<";
	else if(kind == ND_RINEEQU) return "<=";
	else if(kind == ND_LEFTINE) return ">";
	else if(kind == ND_LINEEQU)	return ">=";
	else if(kind == ND_ASSIGN)	return "=";
	else if(kind == ND_LVAR)	return "locvar";
	else if(kind == ND_SEMICORO)	return ";";
	else if(kind == ND_RETURN)	return "return";
	else if(kind == ND_IF)	return "if";
	else if(kind == ND_ELSE)	return "else";
	else if(kind == ND_WHILE)	return "while";
	else if(kind == ND_FOR)		return "for";
	else if(kind == ND_FOREXPR)	return "forexpr";
	else if(kind == ND_BLOCK)	return "{}";
	else if(kind == ND_APP)	return "app";
	else if(kind == ND_FUN)	return "fun";
	else return "nodekind2str error";
}

char *syntax_debug(Node *code){
	
	char *str[2];
	char *stra = calloc(1, sizeof(char));
	Nodekind kind = code->kind;

	for(int i = 0;i < 2;i++)	str[i] = "NULL";
	if(code->lhs)	str[0] = syntax_debug(code->lhs);
	if(code->lhs)	str[1] = syntax_debug(code->lhs);
	sprintf(stra, "(%s, %s, %s)", nodekind2str(kind), str[0], str[1]);
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
		Node *equ = equarity();
		node = new_node(equ->type->ty, ND_ASSIGN, node, equ);
	}
	return node;

}

Node *equarity(){

	Node *node = relational();

	for(;;){
		if(consume("==")){
			node = new_node(INT, ND_EQU, node, relational());
		}else if(consume("!=")){
			node = new_node(INT, ND_NOTEQU, node, relational());
		}else{
			return node;
		}
	}
}

Node *relational(){

	Node * node = add();

	for(;;){
		if(consume("<")){
			node = new_node(INT, ND_RIGHTINE, node, add());
		}else if(consume("<=")){
			node = new_node(INT, ND_RINEEQU, node, add());
		}else if(consume(">")){
			node = new_node(INT, ND_LEFTINE, node, add());
		}else if(consume(">=")){
			node = new_node(INT, ND_LINEEQU, node, add());
		}else{
			return node;
		}
	}
}

Node *add(){

	Node *node = mul();

	for(;;){
		if(consume("+")){
			node = new_node(INT, ND_ADD, node, mul());
		}else if(consume("-")){
			node = new_node(INT, ND_SUB, node, mul());
		}else{
			return node;
		}
	}
}

Node *mul(){

	Node *node = unary();

	for(;;){
		if(consume("*")){
			node = new_node(INT, ND_MUL, node, unary());
		}else if(consume("/")){
			node = new_node(INT, ND_DIV, node, unary());
		}else{
			return node;
		}
	}
}

Node *unary(){
	if(consume("+")){
		return primary();
	}else if(consume("-")){
		return new_node(INT, ND_SUB, new_node_num(0), primary());
	}else if(consume("&")){
		Node *lhs = unary();
		Node *node = new_node(ADDR, ND_ADDR, lhs, NULL);
		lhs->par = node;
		return node;
	}else if(consume("*")){
		Node *node = calloc(1, sizeof(Node));
		node->kind = ND_DEREF;
		node->type = calloc(1, sizeof(Type));
		node->type->ty = PTR;
		Type *type = node->type;
		int ptr_size = 1;
		for(;consume("*");type = type->ptr_to){
			type->ptr_to = calloc(1, sizeof(Type));
			type->ptr_to->ty = PTR;
			ptr_size++;
		}
		node->lhs = unary();
		node->lhs->par = node;
		type->ptr_to = node->lhs->type;
		node->lhs->type->ptr_size = ptr_size;
		return node;
	}else if(consume("sizeof")){
		Node *rhs = unary();
		Node *typenode = find_tree_type(rhs);
		// typenode == NULL はND_NUMを表す
		if(!typenode || typenode->kind == ND_ADDR || typenode->kind == ND_DEREF)	return new_node_num(8);
		else if(typenode->kind == ND_LVAR || typenode->kind == ND_APP || typenode->kind == ND_GBLVAR){
			if(!typenode->type)	error_at(token->str, "パーズで変数に型がありません.");
			else if(typenode->type->ty == ARRAY_INT)	return new_node_num(typenode->type->array_size*4);
			else if(typenode->type->ty == ARRAY_CHAR)	return new_node_num(typenode->type->array_size*1);
			else if(typenode->par && (typenode->par->type->ty == PTR || typenode->par->type->ty == ADDR))	return new_node_num(8);
			else if(typenode->defnode && typenode->defnode->par && 
					(typenode->defnode->par->type->ty == PTR || typenode->defnode->par->type->ty == ADDR))	return new_node_num(8);
			else if(typenode->type->ty == INT)	return new_node_num(4);
			else if(typenode->type->ty == CHAR)	return new_node_num(1);
			else if(typenode->type->ty == PTR)	return new_node_num(8);
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
	}else if(consume("\"")){
		String *string = calloc(1, sizeof(String));
		string->len = token->len;
		string->str = token->str;
		if(string_s){
			string_e->next = string;
		}else{
			string_s = string;
		}
		string_e = string;
	}
	Node *par = calloc(1, sizeof(Node));
	Type *this_type = calloc(1, sizeof(Type));
	int ptr_size = 0;
	int type_select = 0;
	// 型定義がある場合の処理
	if(token->len == 3 && strncmp(token->str, "int", token->len) == 0)	type_select = 1;
	else if(token->len == 4 && strncmp(token->str, "char", token->len) == 0)	type_select = 2;
	if(type_select > 0){
		token = token->next;
		Type *type = this_type;
		for(;consume("*");type = type->ptr_to){
			type->ty = PTR;
			type->ptr_to = calloc(1, sizeof(Type));
			ptr_size++;
		}
		if(type_select == 1)	type->ty = INT;
		else if(type_select == 2)	type->ty = CHAR;
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
		if(par->kind == ND_DEREF){
			par->lhs = calloc(1, sizeof(Node));
			node->par = par;
		}
		/* 変数の型を決める(定義時のみ) */
		node->type = calloc(1, sizeof(Type));
		if(type_select == 1)	node->type->ty = INT;
		else if(type_select == 2)	node->type->ty = CHAR;
		node->type->ptr_size = ptr_size;
		/* ===== */
		Token *token2 = token;
		token = token->next;
		/* 変数名の保存*/
		strncpy(node->varname, token2->str, token2->len);
		*(node->varname+token2->len) = '\0';
		/* 関数 */
		if(consume("(")){
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
				if(type_select == 0)	error_at(token->str, "関数の型が定義されていません.");
				// =====
				if(!consume(")")){
					while(1){
						Ty ty;
						if(strncmp(token->str, "int", token ->len) == 0)	ty = INT;
						else if(strncmp(token->str, "char", token->len) == 0)	ty = CHAR;
						else	error_at(token->str, "関数定義の引数の型が不正です.");
						token = token->next;
						Token *tok = consume_ident();
						if(!tok)	error("関数定義の仮引数が正しくありません.");
						add_lvar(tok, NULL, 1, ty);
						token = token->next;
						if(!consume(")"))	expect(',');
						else 	break;
					}
				}
				// 関数定義の名前の追加
				LVar *funlvar = calloc(1, sizeof(LVar));
				funlvar->name = tok->str;
				funlvar->len = tok->len;
				funlvar->defnode = node;
				if(!function_set_s){
					function_set_s = function_set_e = funlvar;
				}else{
					function_set_e->next = funlvar;
					function_set_e = funlvar;
				}
				/* ブロック内の処理 */
				node->lhs = stmt();
				// グローバル環境に変更
				cur_node = NULL;
				return node;
			/* 関数適用の場合 */
			}else if(node->kind == ND_APP){
				if(type_select > 0)	error_at(token->str, "関数適用で型の定義はできません.");
				// 関数が存在するか確認
				LVar *lvar = function_set_s;
				for(;lvar;lvar = lvar->next){
					if(lvar->len == tok->len && strncmp(lvar->name, tok->str, tok->len) == 0) break;
				}
				if(!lvar)	error_at(token->str, "定義されていない関数の参照です.");
				// 型の決定
				node->type->ty = lvar->defnode->type->ty;
				// =====
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
			if(type_select > 0){
				int size = expect_number();
				expect(']');
				par->kind = ND_DEREF;
				par->type = calloc(1, sizeof(Type));
				par->type->ty = PTR;
				par->type->ptr_to = this_type;
				if(type_select == 1)	node->type->ty = ARRAY_INT;
				else if(type_select == 2)	node->type->ty = ARRAY_CHAR;
				node->type->array_size = size;
				par->lhs = node;
				node->par = par;
				if(!cur_node){
					node->kind = ND_GBLVAR;
					add_gblvar(tok, node);
				}else{
					node->kind = ND_LVAR;	
					add_lvar(tok, node, 0, node->type->ty);
				}
				return node;
			}else{
				Node *idnode = add();
				expect(']');
				LVar *lvar;
				// ローカルorグローバル変数にあるか判定
				lvar = find_lvar(tok, cur_node);
				if(lvar)	node->kind = ND_LVAR;
				if(!lvar)	{
					lvar = find_gblvar(tok);
					if(lvar)	node->kind = ND_GBLVAR;
				}
				if(!lvar)	error_at(tok->str, "変数が定義されていません.");
				// =====
				par->kind = ND_DEREF;
				par->type = calloc(1, sizeof(Type));
				par->type->ty = PTR;
				node->type->ty = lvar->defnode->type->ty;
				node->offset = lvar->offset;
				node->defnode = lvar->defnode;
				Node *newnode = new_node(INT, ND_ADD, node, idnode);
				par->type->ptr_to = newnode->type;
				par->lhs = newnode;
				return par;
			}
		}
		// 変数
		LVar *lvar;
		// グローバル環境のとき
		if(!cur_node){
			if(type_select > 0){
				add_gblvar(tok, node);
				node->kind = ND_GBLVAR;
				return node;
			}else{
				error_at(tok->str, "グローバル環境で変数の参照はできません.\n");
			}
		// 関数環境のとき
		}else{
			lvar = find_lvar(tok, cur_node);
			if(lvar)	node->kind = ND_LVAR;
			// 変数定義のとき
			if(type_select > 0){
				if(!lvar){
					add_lvar(tok, node, 0, node->type->ty);
					node->kind = ND_LVAR;
					return node;
				}else{
					error_at(tok->str, "変数定義が重複しています.\n");
				}
			}
			// グローバル変数の参照
			if(!lvar){
				lvar = find_gblvar(tok);
				if(lvar)	node->kind = ND_GBLVAR;
			}
			if(lvar){
				node->offset = lvar->offset;
				node->defnode = lvar->defnode;
				if(lvar->defnode)	{						// 関数定義の引数にはdefnodeは存在しない
					node->type->ty = lvar->defnode->type->ty;	
					node->type->array_size = lvar->defnode->type->array_size;
				}
				return node;
			}else{
				error_at(token->str, "定義されていない変数の参照です.");
			}
		}
	}else if(type_select > 0){
		error_at(token->str, "変数以外に型はつけられません.");
	}
	return new_node_num(expect_number());
}