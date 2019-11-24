#include <stdio.h>
#include <string.h>


int fib(int n){
	if(n == 0)	0;
	else 	fib(n-1)+1;
}


int main(){
	fib(10);
}