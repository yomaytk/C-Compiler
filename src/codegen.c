#include<stdio.h>
#include"mss9cc.h"

void gen_lval(Node *node){
	if(node->kind != ND_LVAR){
		error("左辺値が変数ではありません.");
	}
	
	// store address of variable to stack
	printf("\tmov\trax, rbp\n");
	printf("\tsub\trax, %d\n", node->offset);
	printf("\tpush\trax\n");
}

void gen(Node *node){

	Nodekind kind = node->kind;

	if(kind == ND_RETURN){
		gen(node->lhs);
		printf("\tpop\trax\n");
		printf("\tmov\trsp, rbp\n");
		printf("\tpop\trbp\n");
		printf("\tret\n");
		return;
	}

	if(kind == ND_IF){
		gen(node->lhs);
		printf("\tpop\trax\n");
		printf("\tcmp\trax, 0\n");
		printf("\tje\t.Lelse\n");
		if(node->rhs->kind == ND_ELSE){
			gen(node->rhs->lhs);
			printf("\tjmp\t.Lend\n");
			printf(".Lelse:\n");
			gen(node->rhs->rhs);
			printf(".Lend:\n");
		}else{
			gen(node->rhs);
			printf(".Lelse:\n");
		}
		return;
	}

	if(kind == ND_WHILE){
		printf(".Lwhile_begin:\n");
		gen(node->lhs);
		printf("\tpop\trax\n");
		printf("\tcmp\trax, 0\n");
		printf("\tje\t.Lwhile_end\n");
		gen(node->rhs);
		printf("\tjmp\t.Lwhile_begin\n");
		printf(".Lwhile_end:\n");
		return;
	}

	if(kind == ND_FOR){
		if(node->lhs->lhs){
			gen(node->lhs->lhs);
		}
		printf(".Lfor_begin:\n");
		if(node->lhs->mhs){
			gen(node->lhs->mhs);
			printf("\tpop\trax\n");
			printf("\tcmp\trax, 0\n");
			printf("\tje\t.Lfor_end\n");
		}
		gen(node->rhs);
		if(node->lhs->rhs){
			gen(node->lhs->rhs);
		}
		printf("\tjmp\t.Lfor_begin\n");
		printf(".Lfor_end:\n");
		return;
	}

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
	}else if(kind == ND_BLOCK){
		Node *vec = node->vector;
		while(vec){
			gen(vec);
			printf("\tpop\trax\n");
			vec = vec->vector;
		}
		return;
	}

	gen(node->lhs);
	gen(node->rhs);

	printf("\tpop\trdi\n");
	printf("\tpop\trax\n");

	if(kind == ND_ADD){					// +
		printf("\tadd\trax, rdi\n");
	}else if(kind == ND_SUB){			// -
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