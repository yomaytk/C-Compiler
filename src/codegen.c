#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mss9cc.h"

char *reg64_name[6] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};

void fun_params_err(){
	fprintf(stderr, "仮引数が正しくありません.");
	exit(1);
}

void gen_lval(Node *node){
	if(node->kind == ND_LVAR){
		// store address of variable to stack
		printf("\tmov\trax, rbp\n");
		printf("\tsub\trax, %d\n", node->offset);
		printf("\tpush\trax\n");
		return;
	}
	if(node->kind == ND_DEREF){
		Node *lhs_term = node;
		while(lhs_term->lhs)	lhs_term = lhs_term->lhs;
		if(lhs_term->type && lhs_term->type->ty == ARRAY){
			// printf("sssssss\n");
			gen_lval(lhs_term);
			lhs_term->kind = ND_NUM;
			lhs_term->val = 0;
			// printf("mmmmmmm\n");
			if(node->lhs->rhs){
				gen(node->lhs);
				printf("\tpop\trdi\n");
				printf("\tpop\trax\n");
				printf("\timul\trdi, 8\n");
				printf("\tadd\trax, rdi\n");
				printf("\tpush\trax\n");
			}
			// printf("eeeeeee\n");
		}else 	gen(node->lhs);
		return;
	}
	error("左辺値が変数ではありません.");
}

void gen(Node *node){

	Nodekind kind = node->kind;

	if(kind == ND_NUM){
		printf("\tpush\t%d\n", node->val);
		return;
	}else if(kind == ND_LVAR){
		gen_lval(node);
		
		// push number of variable, using address of variable
		printf("\tpop\trax\n");
		printf("\tmov\trax, [rax]\n");
		printf("\tpush\trax\n");
		return;
	}else if(kind == ND_ASSIGN){
		gen_lval(node->lhs);
		gen(node->rhs);

		printf("\tpop\trdi\n");	// result of rvalue
		printf("\tpop\trax\n");	// address of lvalue
		printf("\tmov\t[rax], rdi\n");
		printf("\tpush\trdi\n");
		return;
	}

	if(kind == ND_RETURN){
		gen(node->lhs);
		printf("\tpop\trax\n");
		printf("\tmov\trsp, rbp\n");
		printf("\tpop\trbp\n");
		printf("\tret\n");
		return;
	}

	if(kind == ND_IF){
		int endid = ++label_cnt;
		if(!node->rhs){
			gen(node->lhs);
			printf("\tpop\trax\n");
			printf("\tcmp\trax, 0\n");
			printf("\tje\t.Lifend%d\n", endid);
			gen(node->mhs);
			printf(".Lifend%d:\n", endid);
			return;
		}

		int lcnt = 0;
		for(;node && node->kind == ND_IF;node = node->rhs){
			gen(node->lhs);
			printf("\tpop\trax\n");
			printf("\tcmp\trax, 0\n");
			printf("\tje\t.Lifelse%d\n", node->labelcnt[++lcnt] = ++label_cnt);
			gen(node->mhs);
			printf("\tjmp\t.Lifend%d\n", endid);
			printf(".Lifelse%d:\n", node->labelcnt[lcnt]);
		}
		if(node && node->kind == ND_ELSE){
			gen(node->rhs);
		}
		printf(".Lifend%d:\n", endid);
		return;
	}

	if(kind == ND_WHILE){
		printf(".Lwhile_begin%d:\n", node->labelcnt[0] = ++label_cnt);
		gen(node->lhs);
		printf("\tpop\trax\n");
		printf("\tcmp\trax, 0\n");
		printf("\tje\t.Lwhile_end%d\n", node->labelcnt[1] = ++label_cnt);
		gen(node->rhs);
		printf("\tjmp\t.Lwhile_begin%d\n", node->labelcnt[0]);
		printf(".Lwhile_end%d:\n", node->labelcnt[1]);
		return;
	}

	if(kind == ND_FOR){
		if(node->lhs->lhs){
			gen(node->lhs->lhs);
		}
		printf(".Lfor_begin%d:\n", node->labelcnt[0] = ++label_cnt);
		if(node->lhs->mhs){
			gen(node->lhs->mhs);
			printf("\tpop\trax\n");
			printf("\tcmp\trax, 0\n");
			printf("\tje\t.Lfor_end%d\n", node->labelcnt[1] = ++label_cnt);
		}
		gen(node->rhs);
		if(node->lhs->rhs){
			gen(node->lhs->rhs);
		}
		printf("\tjmp\t.Lfor_begin%d\n", node->labelcnt[0]);
		printf(".Lfor_end%d:\n", node->labelcnt[1]);
		return;
	}

	if(kind == ND_BLOCK){
		Node *vec = node->vector;
		while(vec){
			gen(vec);
			vec = vec->vector;
			printf("\tpop\trax\n");
		}
		return;
	}

	if(kind == ND_APP){
		Node *vec = node;
		int paramscnt = 0;
		for(;vec->params;vec = vec->params)	paramscnt++;
		// rsp_16n(node, paramscnt);
		for(vec = node;vec->params;vec = vec->params)	gen(vec->params);
		for(int i = paramscnt;i >= 1;i--){
			printf("\tpop\t%s\n", reg64_name[i-1]);
		}
		printf("\tcall\t%s\n", node->token);
		printf("\tpush\trax\n");
		return;
	}

	if(kind == ND_FUN){
		printf("%s:\n", node->token);
		printf("\tpush\trbp\n");
		printf("\tmov\trbp, rsp\n");
		if(strncmp(node->token, "main", 4) == 0){
			main_flag = 1;
		}
		printf("\tsub\trsp, %d\n", node->var_size);
		LVar *lvar = node->locals_s;
		for(int i = 1;i <= node->params_cnt;i++, lvar = lvar->next){
			printf("\tmov\t[rbp-%d], %s\n", i*8, reg64_name[i-1]);
		}
		gen(node->lhs);
		printf("\tmov\trsp, rbp\n");
		printf("\tpop\trbp\n");
		printf("\tret\n");
		return;
	}

	if(kind == ND_ADDR){
		gen_lval(node->lhs);
		return;
	}

	if(kind == ND_DEREF){
		gen(node->lhs);
		Type *type = node->type;
		Node *lhs_term = node;
		while(lhs_term->lhs)	lhs_term = lhs_term->lhs;
		if(lhs_term->type->ty == ARRAY)	type = type->ptr_to;
		for(;type && type->ty == PTR;type = type->ptr_to){
			printf("\tpop\trax\n");
			printf("\tmov\trax, [rax]\n");
			printf("\tpush\trax\n");
			if(!type->ptr_to)	break;
		}
		return;
	}

	gen(node->lhs);
	gen(node->rhs);

	printf("\tpop\trdi\n");
	printf("\tpop\trax\n");

	if(kind == ND_ADD){					// +
		Node *lhs_term = node;
		while(lhs_term->lhs)	lhs_term = lhs_term->lhs;
		if(lhs_term->defnode && lhs_term->defnode->par && lhs_term->defnode->par->kind == ND_DEREF){
			Type *par_type = lhs_term->defnode->par->type;
			if(par_type->ty == PTR)	printf("\timul\trdi, 8\n");
			else if(par_type->ty == INT)	printf("\timul\trdi, 8\n");
		}
		printf("\tadd\trax, rdi\n");
	}else if(kind == ND_SUB){			// -
		Node *lhs_term = node;
		while(lhs_term->lhs)	lhs_term = lhs_term->lhs;
		if(lhs_term->defnode && lhs_term->defnode->par && lhs_term->defnode->par->kind == ND_DEREF){
			Type *par_type = lhs_term->defnode->par->type;
			if(par_type->ty == PTR)	printf("\timul\trdi, 8\n");
			else if(par_type->ty == INT)	printf("\timul\trdi, 8\n");
		}
		printf("\tsub\trax, rdi\n");
	}else if(kind == ND_MUL){			// *
		printf("\timul\trax, rdi\n");
	}else if(kind == ND_DIV){			// /
		printf("\tcqo\n");
		printf("\tidiv\trdi\n");
	}else if(kind == ND_EQU){			// ==
		printf("\tcmp\trax, rdi\n");
		printf("\tsete\tal\n");
		printf("\tmovzb\trax, al\n");
	}else if(kind == ND_NOTEQU){			// !=
		printf("\tcmp\trax, rdi\n");
		printf("\tsetne\tal\n");
		printf("\tmovzb\trax, al\n");
	}else if(kind == ND_RIGHTINE){			// <
		printf("\tcmp\trax, rdi\n");
		printf("\tsetl\tal\n");
		printf("\tmovzb\trax, al\n");
	}else if(kind == ND_RINEEQU){			// <=
		printf("\tcmp\trax, rdi\n");
		printf("\tsetle\tal\n");
		printf("\tmovzb\trax, al\n");
	}else if(kind == ND_LEFTINE){			// >
		printf("\tcmp\trdi, rax\n");
		printf("\tsetl\tal\n");
		printf("\tmovzb\trax, al\n");
	}else if(kind == ND_LINEEQU){			// >=
		printf("\tcmp\trdi, rax\n");
		printf("\tsetle\tal\n");
		printf("\tmovzb\trax, al\n");
	}

	printf("\tpush\trax\n");
}

void rsp_16n(Node *node, int paramscnt){

	printf("\tpush\trax\n");
	printf("\tmov\trax, rsp\n");
	printf("\tsub\trax, %d\n", paramscnt);
	printf("\tand\trax, 0xf\n");
	printf("\tcmp\trax, 0\n");
	printf("\tpop\trax\n");
	printf("\tje\t.Lrsp_eql_16n%d\n", node->labelcnt[0] = ++label_cnt);
	printf("\tpush\t0xffff\n");
	printf(".Lrsp_eql_16n%d:\n", node->labelcnt[0]);

	return;
}