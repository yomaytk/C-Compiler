#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mss9cc.h"

// 次のトークンが期待している記号のときには、トークンを1つ読み進めて
// 真を返す。それ以外の場合には偽を返す。
bool consume(char *op) {
	// if (token->kind != TK_RESERVED || token->str[0] != op)
	if(token->kind != TK_RESERVED || token->len != strlen(op) || memcmp(token->str, op, token->len)){
		return false;
	}
	token = token->next;
	return true;
}

Token *consume_ident(){
	
	if(token->kind != TK_IDENT || token->str[0] > 'z' || token->str[0] < 'a'){
		return NULL;
	}
	token = token->next;
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

// 新しいトークンを作成してcurに繋げる
Token *new_token(TokenKind kind, Token *cur, char *str, int len) {
	Token *tok = calloc(1, sizeof(Token));
	tok->kind = kind;
	tok->str = str;
	if(len > 0)	tok->len = len;
	cur->next = tok;
	return tok;
}

// 入力文字列pをトークナイズしてそれを返す
Token *tokenize(char *p) {
	Token head;
	head.next = NULL;
	Token *cur = &head;

	while (*p) {
	// 空白文字をスキップ
		if (isspace(*p)) {
			p++;
			continue;
		}

		if('a' <= *p && *p <= 'z'){
			cur = new_token(TK_IDENT, cur, p++, 1);
			continue;
		}

		if (*p == '+' || *p == '-' || *p == '*' || *p == '/' 
			|| *p == '(' || *p == ')' || *p == ';') {
			cur = new_token(TK_RESERVED, cur, p++, 1);
			continue;
		}

		if(!strncmp(p, "==", 2) || !strncmp(p, "!=", 2) 
				|| !strncmp(p, ">=", 2) || !strncmp(p, "<=", 2)){
			cur = new_token(TK_RESERVED, cur, p, 2);
			p++;
			p++;
			continue;
		}

		if(*p == '<' || *p == '>' || *p == '='){
			cur = new_token(TK_RESERVED, cur, p++, 1);
			continue;
		}

		if (isdigit(*p)) {
			cur = new_token(TK_NUM, cur, p, -1);
			cur->val = strtol(p, &p, 10);
			continue;
		}

		error_at(p, "トークナイズできません");
	}

	new_token(TK_EOF, cur, p, -1);
	return head.next;
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
	
	Node *node = expr();
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
		printf("ffffff");
		Node *node = calloc(1, sizeof(Node));
		node->kind = ND_LVAR;
		node->offset = (tok->str[0] - 'a' + 1)*8;
		return node;
	}

	return new_node_num(expect_number());
}