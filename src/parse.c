#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

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

Node *expr(){
	
	Node *node = mul();
	// printf("fefefefe;;;\n");

	for(;;){
		if(consume('+')){
			node = new_node(ND_ADD, node, mul());
		}else if(consume('-')){
			node = new_node(ND_SUB, node, mul());
		}else{
			return node;
		}
	}
}

Node *mul(){

	Node *node = primary();
	// printf("jijijij;;;\n");

	for(;;){
		if(consume('*')){
			node = new_node(ND_MUL, node, primary());
		}else if(consume('/')){
			node = new_node(ND_DIV, node, primary());
		}else{
			return node;
		}
	}
}

Node *primary(){

	// printf("nmnmnmnmn;;;\n");

	if(consume('(')){
		Node *node = expr();
		expect(')');
		return node;
	}

	return new_node_num(expect_number());
}

void gen(Node *node){

	Nodekind kind = node->kind;

	if(kind == ND_NUM){
		printf("\tpush\t%d\n", node->val);
		return;
	}

	gen(node->lhs);
	gen(node->rhs);

	printf("\tpop\trdi\n");
	printf("\tpop\trax\n");

	if(kind == ND_ADD){
		printf("\tadd\trax, rdi\n");
	}else if(kind == ND_SUB){
		printf("\tsub\trax, rdi\n");
	}else if(kind == ND_MUL){
		printf("\timul\trax, rdi\n");
	}else{
		printf("\tcqo\n");
		printf("\tidiv\trdi\n");
	}

	printf("\tpush\trax\n");
}