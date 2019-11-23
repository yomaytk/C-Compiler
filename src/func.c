#include<stdio.h>

int foo() { printf("call function OK!\n"); return 123;}

int hoge(int a, int b){
	printf("%d", a+b);
	return a+b;
}

int hogehoge1(int a, int b, int c, int d, int e, int f){
	printf("hogehoge called!\n");
	printf("hogehoge = %d!\n", a+b+c+d+e+f);
}

int hogehoge2(int a, int b, int c, int d, int e){
	printf("hogehoge called!\n");
	printf("hogehoge = %d!\n", a+b+c+d+e);
}

int hogehoge3(int a, int b, int c, int d){
	printf("hogehoge called!\n");
	printf("hogehoge = %d!\n", a+b+c+d);
}

int hogehoge4(int a, int b, int c){
	printf("hogehoge called!\n");
	printf("hogehoge = %d!\n", a+b+c);
}

int hogehoge5(int a, int b){
	printf("hogehoge called!\n");
	printf("hogehoge = %d!\n", a+b);
}

int hogehoge6(int a){
	printf("hogehoge called!\n");
	printf("hogehoge = %d!\n", a);
}

int hogehogeend(){
	printf("hogehoge called!\n");
	printf("hogehoge = success!!!\n");
}