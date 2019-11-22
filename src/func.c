#include<stdio.h>

int foo() { printf("call function OK!\n"); return 123;}

int hogehoge(int a, int b){
	printf("hogehoge called!\n");
	printf("hogehoge = %d!", a+b);
}