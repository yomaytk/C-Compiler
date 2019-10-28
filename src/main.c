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


int main(int argc, char **argv) {

	if (argc != 2) {
		error_at(token->str, "引数の個数が正しくありません");
		return 1;
	}

	user_input = argv[1];
	token = tokenize(argv[1]);
	Node *node = expr();

	printf(".intel_syntax noprefix\n");
	printf(".global main\n");
	printf("main:\n");

	gen(node);

	printf("\tpop\trax\n");
	printf("\tret\n");
	return 0;
}