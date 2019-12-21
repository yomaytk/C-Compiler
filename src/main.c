#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "mss9cc.h"


// current token
Token *token;

// input program
char *user_input;

// label number for uniqueness
int label_cnt = 0;

int main_flag = 0;

Node *ignore;

// notice price of error
void error_at(char *loc, char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	int pos = loc - user_input;
	fprintf(stderr, "%s\n", user_input);
	fprintf(stderr, "%*s", pos, ""); // pos個の空白を出力
	fprintf(stderr, "^ ");
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	exit(1);
}

void error(char *str){

	fprintf(stderr, "%s\n", str);
	exit(1);

}

int main(int argc, char **argv) {

	if (argc != 2) {
		error_at(token->str, "引数の個数が正しくありません");
		return 1;
	}

	user_input = argv[1];
	token = tokenize(argv[1]);
	// debug of lexer
	// { tokenize_debug(token); return 0; }
	ignore = calloc(1, sizeof(Node));
	program();
	printf(".intel_syntax noprefix\n");

	int j = 1;
	for(String *string = string_s;string;string = string->next, j++){
		printf(".data\n");
		printf(".L.str%d:\n", j);
		printf("\t.ascii \"%.*s\\000\"\n\n", string->len, string->str);
	}

	int i = 0;
	for(;code[i] && code[i]->kind != ND_FUN;i++){
		if(code[i] == ignore)	continue;
		gen(code[i]);
	}
	printf(".text\n");
	printf(".global main\n\n");
	for(;code[i];i++){
		gen(code[i]);
		// printf("\tpop\trax\n");
	}
	
	if(!main_flag)	error("main関数がありません.");

	return 0;
}