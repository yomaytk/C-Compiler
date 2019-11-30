#!/bin/bash
try() {
	expected="$1"
	input="$2"

	./mss9cc "$input" > compile1.s
	gcc -o compile compile1.s
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

try 20 "int main(){ int a = 20; return a;}"
try 10 "int main(){ return 10; }"
try 20 "int main(){ int a = 5; int b = 4; int c = 11; return a+b+c; }"
try 10 "int hoge(){ return 10; } int main(){ hoge(); }"
try 28 "int hoge(int a){ return a+5; } int main(){ hoge(23); }"
try 23 "int hoge1(int a) { return a; } int hoge2(int a) { return a;} int hoge3(int a){ return a;} int main(){ hoge1(10) + hoge2(7) + hoge3(6); }"
try 24 "int hoge(int a, int b){ return a+b; } int main(){ hoge(17, 7); }"
try 22 "int hoge(int a, int b, int c, int d){ a+b+c+d; } int main(){ return hoge(5, 5, 10, 2); }"
try 20 "int hoge(int a, int b, int c){ a+b+c; } int main(){ return hoge(5, 5, 10); }"
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
try 3 "int main(){int x = 3;int y = &x;return *y;}"
try 3 "int main(){int x = 3;int y = 5;int z = &y + 8;return *z;}"
try 6 "int main(){ int x; x = 6; return x; }"
try 3 "int main(){int x;int *y;y = &x;*y = 3;return x;}"
try 5 "int main(){int x;int *y;int **z; x = 5; y = &x; z = &y; return **z;}"
try 7 "int main(){ int w; int *x; int **y; int ***z; w = 7; x = &w; y = &x; z = &y; return ***z;}"

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

