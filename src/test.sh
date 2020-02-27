#!/bin/bash
try() {
	expected="$1"
	input="$2"

	./mss9cc "$input" > compile1.s
	gcc -static -o compile compile1.s
	./compile
	actual="$?"

	if [ "$actual" = "$expected" ]; then
		echo "$input => $actual"
	else
		echo -e "\e[31mFAILED\e[m"
		echo "$input => $expected expected, but got $actual"
		exit 1
	fi
}

compile_err() {
	input="$1"

	echo "$input"
	./mss9cc "$input" > compile2.s
	if [ $? -gt 0 ]; then
		:
	else
		echo -e "\e[31mFAILED\e[m"
		echo "$input"
		echo "must be abort"
		exit 1
	fi
	
}

echo -e "\n\e[32m=== try test start ===\e[m\n"

# basic syntax and various functions
try 21 "int main(){ int a; a = 20; return (a+1);}"
try 10 "int main(){ return 10; }"
try 8  "int main(){ int a = 2; int b = 6; return a+b;}"
try 60 "int main(){ int a = 5; int b = 4; int c = 13; int d = 15; int e = 23; return a+b+c+d+e; }"
try 10 "int hoge(){ return 10; } int main(){ hoge(); }"
try 28 "int hoge(int a){ return a+5; } int main(){ hoge(23); }"
try 23 "int hoge1(int a) { return a; } int hoge2(int a) { return a;} int hoge3(int a){ return a;} int main(){ hoge1(10) + hoge2(7) + hoge3(6); }"
try 24 "int hoge(int a, int b){ return a+b; } int main(){ hoge(17, 7); }"
try 22 "int hoge(int a, int b, int c, int d){ a+b+c+d; } int main(){ return hoge(5, 5, 10, 2); }"
try 20 "int hoge(int a, int b, int c){ a+b+c; } int main(){ int a = 5; return hoge(a, 5, 10); }"
try 71 "int hoge(int a, int b){ return a+b; } int main(){ hoge(5, 66); }"
try 23 "int main(){if (3 > 2) 23; else 33;}"
try 23 "int main(){int a = 10; int b = 23; if(a+b+5 == 38) return 23;}"
try 23 "int main(){ if(0) {return 13;} else {return 23;}}"
try 33 "int main(){if (0) return 13; else if(1) return 33; else if(1) return 76; return 45;}"
try 33 "int main(){if (0) { return 13; } else if(0) return 23; else if(1) return 33; else if(0) { 43; } else if(0) { return 53; } else 63; }"
try 23 "int main(){ if(1 < 0) 13; else if(1 < 3) 23; }"
try 33 "int hoge1(int a, int b, int c) { a+b+c; } int hoge2(int a, int b, int c, int d){ a+b+c+d; } int main(){ hoge1(10, 7, 3)+hoge2(1, 2, 3, 7); }"
try 34 "int hoge1(int a, int b, int c) {a+b+c;} int hoge2(int a, int b, int c, int d){a+b+c+d;} int hoge3(int a, int b, int c, int d, int e, int f){a+b+c+d+e+f;} int main(){return hoge1(2, 4, 4)+hoge2(1, 1, 5, 3)+hoge3(2, 2, 4, 4, 1, 1);}"
try	10 "int fib(int n){ if(n == 0) return 0; else fib(n-1)+1; } int main(){ return fib(10); }"
try 55 "int fib(int n){ if(n == 0) 0; else if(n == 1) return 1; else fib(n-1)+fib(n-2);} int main(){ return fib(10); }"
try 5 "int main(){ int a = 3; {a = 5; return a;} }"
try 55 "int main(){ int ans = 0; for(int i = 0;i <= 10;i = i + 1){ ans = ans + i;} return ans;}"
try 55 "int sum(int m, int n){ int acc = 0; for (int i = m; i <= n; i = i + 1) acc = acc + i; return acc; } int main() { return sum(1, 10);}"

# pointer
try 3 "int main(){int x = 3;int *y = &x;return *y;}"
try 3 "int main(){int x;int *y;y = &x;*y = 3;return x;}"
try 5 "int main(){int x;int *y;int **z; x = 5; y = &x; z = &y; return **z;}"
try 13 "int main(){ int x = 13; int *y = &x; int **z = &y; return **z;}"
try 12 "int main(){ int x = 12; return *(&x); }"
try 7 "int main(){ int w; int *x; int **y; int ***z; w = 7; x = &w; y = &x; z = &y; return ***z;}"
try 12 "int main(){ int w = 13; int x = 12; int *y; y = &w; y = y-1; return *y;}"
try 45 "int main(){ int w = 20; int x = 21; int y = 22; int z = 23; int *p; p = &w; p = p-3; return *p + *(p + 1);}"
try 20 "int main(){ int x = 11; int y = 12;int *p; p = &x; *(p - 1) = 20; return *(p - 1);}"
try 13 "int main(){ int x = 8; int *y; y = &x; return *y - 1 + 2*3;}"

# sizeof
try 8 "int main(){ int *x; sizeof (x+1+5*8);}"
try 8 "int main(){ int x; sizeof(&x);}"
try 4 "int hoge(int a){ return a;}int main(){ sizeof(hoge(10)); }"
try 4 "int main(){ int x; sizeof(x+5*5);}"
try 40 "int main(){ int a[10]; sizeof a;}"

# array
try 3 "int main(){ int a[10]; a[3] = 3; return a[3];}"
try 17 "int main(){ int a[10]; a[3] = 3; a[4] = 4; a[5] = 5; return a[3] + a[4] + 3 + a[5] + 2;}"
try 4 "int main(){ int b = 2; int a[100]; *a = 1; *(a + 28 - 32 + 21) = 3; return *(a+17) + a[2-b+0];}"
try 6 "int main(){int a[10];*a = 1;*(a + 1) = 2; *(a + 2) = 3; int *p;p = a;return *p + *(p+1)+*(p+2);}"
try 10 "int main(){ int a[100]; a[1 - 6 + 20] = 10; return a[15];}"
try 13 "int main(){ int a[10]; a[3] = 4; a[4] = 9; return *(a+3)+*(a+4);}"

#global variable
try 8 "int a; int b[100]; int main(){a = 4; b[5] = 4; return a + b[5];}"
try 13 "int *p; int x; int main(){x = 5; p = &x; *(p + 1) = 4; *(p + 2) = 4; return *p + *(p+1) + *(p+2);}"
try 13 "int ***p; int **q; int *r; int x; int main(){ x = 13; r = &x; q = &r; p = &q; return ***p; }"
try 7 "int b[100]; int main(){ b[0] = 3; *(b+1) = 4; return b[0]+*(b+1);}"
try 13 "int b[10000]; int main(){ b[0] = 3; b[1] = 2; *(b+2) = 8; return *b + *(b+1) + b[2];}"
try 40 "int b[10]; int main(){ return sizeof(b); }"
try 20 "int a; int *b; int **c; int main(){ return sizeof(a) + sizeof(b) + sizeof(c);}"

# char
try 8 "int main(){ char a = 3; char b = 5; return a+b; }"
try 7 "int main(){ char x[3]; x[0] = 3; x[1] = 5; x[2] = -1; return x[0] + *(x+1) + x[2]; }"
try 4 "int main(){ char *x; char y = 4; x = &y; return *x;}"
try 11 "int hoge(char a, char b){ char c = 4; return a+b+c; }int main(){ return hoge(3, 4); }"
try 6 "int main(){ char a = 2; int b = 4; return a+b;}"
try 13 "int hoge(char a, char b){ return a+b;} char hoge2(int a, char b){ return a*b; }int main(){ return hoge(2,3) + hoge2(2,4);}"
try 5 "int main(){ char x = 5; char *y = &x; char **z = &y; char ***w = &z; return ***w; }"
try 14 "char a; char *p; int q; int main(){ a = 5; q = 9; p = &q; return a + *p; }"

# char literal
try 5 "int main(){ \"hello, world\"; return 5; }"
try 97 "int main(){ char *a = \"abc\"; return a[0]; }"
try 100 "int main(){ char *a = \"abcde\"; return a[0] - *(a+1) + a[4]; }"
try 97 "char *a; int main(){ a = \"iiaii\"; return a[2]; }"
try 98 "char hoge(char *a){ return a[1]; }int main(){ char *a = \"abc\"; return hoge(a); }"

# struct
try 10 "int main(){ struct tag { int a; int b; char c;}; struct tag tag1; return 10; }"
try 10 "struct tag1{ int a; char b; int a; }; int main(){ struct tag1 tag11; struct tag2 { int a; char b; int *p; char *q; struct tag1 *x; }; return 10;}"

echo -e "\n\e[32m=== try test SUCCESS ===\e[m\n"

echo -e "\e[32m=== compile_err test start ===\e[m\n"

compile_err "return 4;"
compile_err "int main(){return j;;;}"
compile_err "int main(){for(a = 0;a < 4;;) if(1) return 5;}"
compile_err "int main(){foo(1,4,);}"
compile_err "int main(){foo(,);}"
compile_err "int main(){foo());}"
compile_err "hoge(a, b,){10;} int main(){1;}"

echo -e "\n\e[32m=== compile_err test SUCCESS ===\e[m"

