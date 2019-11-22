#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mss9cc.h"

int is_alnum(char c) {
	return ('a' <= c && c <= 'z') ||
		('A' <= c && c <= 'Z') ||
		('0' <= c && c <= '9') ||
		(c == '_');
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

void tokenize_debug(Token *tok){
	while(tok){
		char s[100];
		strncpy(s, tok->str, tok->len);
		s[tok->len] = '\0';
		printf("%s\n", s);
		tok = tok->next;
	}
	return;
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

		if(strncmp(p, "return", 6) == 0 && !is_alnum(*(p+6))){
			cur = new_token(TK_RETURN, cur, p, 6);
			p += 6;
			continue;
		}

		if(strncmp(p, "if", 2) == 0 && !is_alnum(*(p+2))){
			cur = new_token(TK_IF, cur, p, 2);
			p += 2;
			continue;
		}

		if(strncmp(p, "else", 4) == 0 && !is_alnum(*(p+4))){
			cur = new_token(TK_ELSE, cur, p, 4);
			p += 4;
			continue;
		}

		if(strncmp(p, "while", 5) == 0 && !is_alnum(*(p+5))){
			cur = new_token(TK_WHILE, cur, p, 5);
			p += 5;
			continue;
		}

		if(strncmp(p, "for", 3) == 0 && !is_alnum(*(p+3))){
			cur = new_token(TK_FOR, cur, p, 3);
			p += 3;
			continue;
		}

		if('a' <= *p && *p <= 'z'){
			int i = 0;
			for(;;i++){
				if(*(p+i) < '0' || ('9' < *(p+i) && *(p+i) < 'a') || 'z' < *(p+i)){
					break;
				}
			}
			cur = new_token(TK_IDENT, cur, p, i);
			p += i;
			continue;
		}

		if (*p == '+' || *p == '-' || *p == '*' || *p == '/' 
			|| *p == '(' || *p == ')' || *p == ';' || *p == '{' 
			|| *p == '}' || *p == ',') {
			cur = new_token(TK_RESERVED, cur, p++, 1);
			continue;
		}

		if(!strncmp(p, "==", 2) || !strncmp(p, "!=", 2) 
				|| !strncmp(p, ">=", 2) || !strncmp(p, "<=", 2)){
			cur = new_token(TK_RESERVED, cur, p, 2);
			p += 2;
			continue;
		}

		if(*p == '<' || *p == '>' || *p == '='){
			cur = new_token(TK_RESERVED, cur, p++, 1);
			continue;
		}

		if (isdigit(*p)) {
			cur = new_token(TK_NUM, cur, p, 1);
			cur->val = strtol(p, &p, 10);
			continue;
		}

		error_at(p, "トークナイズできません");
	}

	new_token(TK_EOF, cur, p, -1);
	return head.next;
}