#include<stdio.h>

int main(){

	int a = 1;
	if(NULL){
		printf("NULL is true");
	}
	// a = NULL;
	printf("%d", a);

	for(int i = 0, j = 0;i < 10;i++, j++){
		printf("%d\t%d\n", i, j);
	}
	printf("return");

	return 0;
}