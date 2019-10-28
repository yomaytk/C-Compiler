#include<stdio.h>
#include"mss9cc.h"

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
	}else if(kind == ND_NEQ){			// !=
		printf("\tcmp\trax, rdi\n");
		printf("\tsetne\tal\n");
		printf("\tmovzb\trax, al\n");
	}else if(kind == ND_RIL){			// <
		printf("\tcmp\trax, rdi\n");
		printf("\tsetl\tal\n");
		printf("\tmovzb\trax, al\n");
	}else if(kind == ND_RLE){			// <=
		printf("\tcmp\trax, rdi\n");
		printf("\tsetle\tal\n");
		printf("\tmovzb\trax, al\n");
	}else if(kind == ND_LIL){			// >
		printf("\tcmp\trdi, rax\n");
		printf("\tsetl\tal\n");
		printf("\tmovzb\trax, al\n");
	}else if(kind == ND_LLE){			// >=
		printf("\tcmp\trdi, rax\n");
		printf("\tsetle\tal\n");
		printf("\tmovzb\trax, al\n");
	}

	printf("\tpush\trax\n");
}