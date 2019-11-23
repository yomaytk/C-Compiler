#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mss9cc.h"


// current token
Token *token;

// input program
char *user_input;

// label number for uniqueness
int label_cnt = 0;

Node *cur_node;
Node *tmp_node;

int main_flag = 0;

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

	cur_node = calloc(1, sizeof(Node));
	tmp_node = cur_node;

	program();
	printf(".intel_syntax noprefix\n");
	printf(".global main\n\n");

	for(int i = 0;code[i];i++){
		gen(code[i]);
		// printf("\tpop\trax\n");
	}
	
	// for(int i = 0;code[i];i++){
	// 	printf("code[%d]:\n", i);
	// 	printf("%s\n", syntax_debug(code[i]));
	// }

	if(!main_flag)	error("main関数がありません.");

	return 0;
}