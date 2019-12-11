#include<stdio.h>
#include<stdlib.h>

int ary[2]; 

struct Sss{
	int a;
	int b;
	struct Sss *sss;
};

int main(){ 
	*ary=1; 
	*(ary+1)=2; 
	struct Sss *sss = calloc(1, sizeof(struct Sss));
	if(sss)	printf("hello!");
	return *ary + *(ary+1);
}