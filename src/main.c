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

void error(char *str){

	fprintf(stderr, "%s\n", str);
	exit(1);

}


int main(int argc, char **argv) {

	if (argc != 2) {
		error_at(token->str, "引数の個数が正しくありません");
		return 1;
	}

	// printf("eee\n");
	user_input = argv[1];
	token = tokenize(argv[1]);
	// printf("rrr\n");
	program();
	printf(".intel_syntax noprefix\n");
	printf(".global main\n");
	printf("main:\n");

	printf("\tpush\trbp\n");
	printf("\tmov\trbp, rsp\n");
	int vararea = 0; 
	LVar *lvar = locals_s;
	while(lvar){
		vararea += 8;
		lvar = lvar->next;
	}
	printf("\tsub\trsp, %d\n", vararea);	// ローカル変数の場所をスタック上に確保しないと、別の値がローカル変数のアドレスに格納される恐れがある。

	for(int i = 0;code[i];i++){
		gen(code[i]);
		printf("\tpop\trax\n");
	}
	printf("\tmov\trsp, rbp\n");
	printf("\tpop\trbp\n");
	printf("\tret\n");
	return 0;
}