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
		printf("\tje\t.Lelse%d\n", node->labelcnt[0] = ++label_cnt);
		if(node->rhs->kind == ND_ELSE){
			gen(node->rhs->lhs);
			printf("\tjmp\t.Lend%d\n", node->labelcnt[1] = ++label_cnt);
			printf(".Lelse%d:\n", node->labelcnt[0]);
			gen(node->rhs->rhs);
			printf(".Lend%d:\n", node->labelcnt[1]);
		}else{
			gen(node->rhs);
			printf(".Lelse%d:\n", node->labelcnt[0]);
		}
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

	if(kind == ND_APP){
		Node *vec = node;
		int paramscnt = 0;
		while(vec->vector){
			gen(vec->vector);
			vec = vec->vector;
			paramscnt++;
		}
		vec = node->vector;
		// rsp_16n(node);
		if(paramscnt == 6){ printf("\tpop\tr9\n"); vec = vec->vector; paramscnt--;}
		if(paramscnt == 5){ printf("\tpop\tr8\n"); vec = vec->vector; paramscnt--;}
		if(paramscnt == 4){ printf("\tpop\trcx\n"); vec = vec->vector; paramscnt--;}
		if(paramscnt == 3){ printf("\tpop\trdx\n"); vec = vec->vector; paramscnt--;}
		if(paramscnt == 2){ printf("\tpop\trsi\n"); vec = vec->vector; paramscnt--;}
		if(paramscnt == 1){ printf("\tpop\trdi\n"); vec = vec->vector; paramscnt--;}
		printf("\tcall\t%s\n", node->token);
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

void rsp_16n(Node *node){

	printf("\tpush\trax\n");
	printf("\tmov\trax, rsp\n");
	printf("\tand\trax, 0xf\n");
	printf("\tcmp\trax, 0\n");
	printf("\tje\t.Lrsp_eql_16n%d\n", node->labelcnt[0] = ++label_cnt);
	printf("\tpop\trax\n");
	printf("\tpush\t0xffff\n");
	printf(".Lrsp_eql_16n%d:\n", node->labelcnt[0]);
	printf("\tpop\trax\n");

	return;
}