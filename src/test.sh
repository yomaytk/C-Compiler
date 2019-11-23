#!/bin/bash
try() {
	expected="$1"
	input="$2"

	./mss9cc "$input" > compile.s
	gcc -o compile compile.s
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
	./mss9cc "$input" > compile.s
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


try 10 "main(){ return 10; }"
try 20 "main(){ a = 5; b = 4; c = 11; return a+b+c; }"
try 10 "hoge(){ return 10; } main(){ hoge(); }"
try 22 "hoge(a, b, c, d){ a+b+c+d; } main(){ return hoge(5, 5, 10, 2); }"
try 20 "hoge(a, b, c){ a+b+c; } main(){ return hoge(5, 5, 10); }"
try 71 "hoge(a, b){ return a+b; } main(){ hoge(5, 66); }"
try 23 "main(){if (3 > 2)	return 23; else 33;}"
try 23 "main(){a = 10; b = 23; if(a+b+5 == 38) return 23;}"
# try	10 "fib(n){ if(n == 1) 1; else fib(n-1)+fib(n-2);} main(){ return fib(10); }"

echo -e "\n\e[32m=== try test SUCCESS ===\e[m\n"

echo -e "\e[32m=== compile_err test start ===\e[m\n"

compile_err "return 4;"
compile_err "main(){return j;;;}"
compile_err "main(){for(a = 0;a < 4;;) if(1) return 5;}"
compile_err "main(){foo(1,4,);}"
compile_err "main(){foo(,);}"
compile_err "main(){foo());}"

echo -e "\n\e[32m=== compile_err test SUCCESS ===\e[m"

