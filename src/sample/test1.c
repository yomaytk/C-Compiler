#include <stdio.h>
#include <string.h>


// int hogehoge(int a, int b){
// 	return a+b;
// }


int main(){
	char *ss = calloc(1, sizeof(char*));
	*ss = 'a';
	*(ss+1) = 'b';
	// ss = foobar();
	printf("%s", ss);
    return 0;
}