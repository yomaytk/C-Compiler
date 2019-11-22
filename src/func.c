#include<stdio.h>

int foo() { printf("call function OK!\n"); return 123;}

int hogehoge(int a, int b, int c, int d, int e, int f){
	printf("hogehoge called!\n");
	printf("hogehoge = %d!\n", a+b+c+d+e+f);
}