#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mss9cc.h"

char *reg64_name[6] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
char *reg32_name[6] = {"edi", "esi", "edx", "ecx", "r8d", "r9d"};
char *reg16_name[6] = {"di", "si", "dx", "cx", "r8w", "r9w"};
char *reg8_name[6] = {"dil", "sil", "dl", "cl", "r8b", "r9b"};

void fun_params_err(){
	fprintf(stderr, "仮引数が正しくありません.");
	exit(1);
}

void ptr_is8n(Node *node){
	// 最左端のノードを見つける
	Node *lhs_term = node;
	while(lhs_term->lhs){
		lhs_term = lhs_term->lhs;
		if(lhs_term->kind == ND_DEREF)	return;
	}
	// =====
	if(lhs_term->defnode && lhs_term->defnode->par && lhs_term->defnode->par->kind == ND_DEREF){
		Node *defnode = lhs_term->defnode;
		int ptr_dif = defnode->type->ptr_size - lhs_term->type->ptr_size;
		if((ptr_dif > 0 && lhs_term->defnode->type->ty == INT) || lhs_term->type->ty == ARRAY_INT){
			printf("\timul\trbx, 4\n");
		}
	}
	return;
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
		gen(node->lhs);
		return;
	}
	error("左辺値が変数ではありません.\n");
}

void gen_gval(Node *node){
	printf("\tlea\trax, %s\n", node->varname);
	printf("\tpush\trax\n");
	return;
}

void gen(Node *node){

	if(node == ignore)	return;
	
	Nodekind kind = node->kind;

	if(kind == ND_STRING)	return;

	if(kind == ND_NUM){
		printf("\tpush\t%d\n", node->val);
		return;
	}else if(kind == ND_LVAR){
		gen_lval(node);
		if(node->type->ty == ARRAY_INT || node->type->ty == ARRAY_CHAR)	return;

		// push number of variable, using address of variable
		printf("\tpop\trax\n");

		if((node->par && node->par->type->ty == PTR) 
			|| (node->defnode && node->defnode->par && node->defnode->par->type->ty == PTR))	printf("\tmov\trax, [rax]\n");
		else if(node->type->ty == CHAR)	printf("\tmovsx\trax, BYTE PTR [rax]\n");
		else if(node->type->ty == INT)	printf("\tmov\teax, [rax]\n");
		else 	printf("\tmov\trax, [rax]\n");
		printf("\tpush\trax\n");

		return;
	}else if(kind == ND_ASSIGN){
		if(node->lhs->kind == ND_GBLVAR)	gen_gval(node->lhs);
		else 	gen_lval(node->lhs);
		if(node->rhs->kind == ND_STRING){
			printf("\tlea\tr10, .L.str%d\n", node->rhs->offset);
			printf("\tpush\tr10\n");
		}else{
			gen(node->rhs);
		}
		printf("\tpop\trbx\n");	// result of rvalue
		printf("\tpop\trax\n");	// address of lvalue

		if((node->lhs->par && node->lhs->par->type->ty == PTR)
			|| (node->lhs->defnode && node->lhs->defnode->par && node->lhs->defnode->par->type->ty == PTR))	printf("\tmov\t[rax], rbx\n");
		else if(node->lhs->type->ty == CHAR)	printf("\tmov\t[rax], bl\n");
		else if(node->lhs->type->ty == INT)	printf("\tmov\t[rax], ebx\n");
		else 	printf("\tmov\t[rax], rbx\n");
		printf("\tpush\trbx\n");
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
			Node *vecc = vec;
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
		printf("\tcall\t%s\n", node->varname);
		printf("\tpush\trax\n");
		return;
	}

	if(kind == ND_FUN){
		printf("%s:\n", node->varname);
		printf("\tpush\trbp\n");
		printf("\tmov\trbp, rsp\n");
		if(strncmp(node->varname, "main", 4) == 0){
			main_flag = 1;
		}
		printf("\tsub\trsp, %d\n", node->var_size);
		LVar *lvar = node->locals_s;
		int params_size = 0;
		for(int i = 1;i <= node->params_cnt;i++, lvar = lvar->next){
			if(lvar->params_ty == INT)	{
				printf("\tmov\t[rbp-%d], %s\n", params_size+4, reg32_name[i-1]);
				params_size += 4;
			}else if(lvar->params_ty == CHAR){
				printf("\tmov\t[rbp-%d], %s\n", params_size+1, reg8_name[i-1]);
				params_size += 1;
			}else if(lvar->params_ty == ARRAY_INT || lvar->params_ty == ARRAY_CHAR || lvar->params_ty == PTR){
				printf("\tmov\t[rbp-%d], %s\n", params_size+8, reg64_name[i-1]);
				params_size += 8;
			}
		}
		gen(node->lhs);
		printf("\tmov\trsp, rbp\n");
		printf("\tpop\trbp\n");
		printf("\tret\n");
		return;
	}

	if(kind == ND_ADDR){
		if(node->lhs->kind == ND_GBLVAR)	gen_gval(node->lhs);
		else 	gen_lval(node->lhs);
		return;
	}

	if(kind == ND_DEREF){
		gen(node->lhs);
		for(Type *type = node->type;type && type->ty == PTR;type = type->ptr_to){
			printf("\tpop\trax\n");
			printf("\tmov\trax, [rax]\n");
			printf("\tpush\trax\n");
			if(!type->ptr_to)	break;
		}
		return;
	}

	if(kind == ND_GBLVAR){
		// 変数定義のとき
		if(!node->defnode){
			printf(".bss\n");
			printf("%s:\n", node->varname);
			if(node->type->ty == ARRAY_INT)	printf("\t.zero %ld\n", node->type->array_size*4);
			else if(node->type->ty == ARRAY_CHAR)	printf("\t.zero %ld\n", node->type->array_size*1);
			else if(node->par && (node->par->type->ty == PTR || node->par->type->ty == ADDR))	printf("\t.zero 8\n");
			else if(node->type->ty == INT)	printf("\t.zero 4\n");
			else if(node->type->ty == CHAR)	printf("\t.zero 1\n");
			return;
		// 変数利用のとき
		}else{
			gen_gval(node);
			if(node->type->ty == ARRAY_INT || node->type->ty == ARRAY_CHAR)	return;

			// push number of global variable, using address of variable
			printf("\tpop\trax\n");
			printf("\tmov\trax, [rax]\n");
			printf("\tpush\trax\n");
			return;
		}
	}

	if(kind == ND_STRING){
		
	}

	gen(node->lhs);
	gen(node->rhs);

	printf("\tpop\trbx\n");
	printf("\tpop\trax\n");

	if(kind == ND_ADD){					// +
		ptr_is8n(node);
		printf("\tadd\trax, rbx\n");
	}else if(kind == ND_SUB){			// -
		ptr_is8n(node);
		printf("\tsub\trax, rbx\n");
	}else if(kind == ND_MUL){			// *
		printf("\timul\trax, rbx\n");
	}else if(kind == ND_DIV){			// /
		printf("\tcqo\n");
		printf("\tidiv\trbx\n");
	}else if(kind == ND_EQU){			// ==
		printf("\tcmp\trax, rbx\n");
		printf("\tsete\tal\n");
		printf("\tmovzb\trax, al\n");
	}else if(kind == ND_NOTEQU){			// !=
		printf("\tcmp\trax, rbx\n");
		printf("\tsetne\tal\n");
		printf("\tmovzb\trax, al\n");
	}else if(kind == ND_RIGHTINE){			// <
		printf("\tcmp\trax, rbx\n");
		printf("\tsetl\tal\n");
		printf("\tmovzb\trax, al\n");
	}else if(kind == ND_RINEEQU){			// <=
		printf("\tcmp\trax, rbx\n");
		printf("\tsetle\tal\n");
		printf("\tmovzb\trax, al\n");
	}else if(kind == ND_LEFTINE){			// >
		printf("\tcmp\trbx, rax\n");
		printf("\tsetl\tal\n");
		printf("\tmovzb\trax, al\n");
	}else if(kind == ND_LINEEQU){			// >=
		printf("\tcmp\trbx, rax\n");
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