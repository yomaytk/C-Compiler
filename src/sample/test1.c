#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum{
	ND_MAIN,
	ND_ADD,			// +
	ND_SUB,			// -
	ND_MUL,			// *
	ND_DIV,			// /
	ND_NUM,			// digit
	ND_EQU,			// ==
	ND_NOTEQU,		// !=
	ND_RIGHTINE,	// <
	ND_RINEEQU,		// <=
	ND_LEFTINE, 	// >
	ND_LINEEQU,		// >=
	ND_ASSIGN,		// =
	ND_LVAR,		// local variable
	ND_SEMICORO,	// ;
	ND_RETURN,		// return
	ND_IF,			// if
	ND_ELSE,		// else
	ND_WHILE,
	ND_FOR,
	ND_FOREXPR,		// condition of for
	ND_BLOCK,		// {}
	ND_APP,			
	ND_FUN,
	ND_ADDR,		// &p
	ND_DEREF,		// *p	
	ND_ARRAY,		// []
} Nodekind;

typedef struct T T;
struct T {
	Nodekind kind;
	int a;
};

int main(){
	T *t;
	if(t)	return 13;
	else 	return 10;
}