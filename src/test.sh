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

# try 47 '5+6*7;'
# try 15 '5*(9-6);'
# try 4 '(3+5)/2;'
# try 179 "(((78))+6-32+ 6 +((58-62)+125)*1/1*2/2);"
# try 10 "-(+20-30-0);"
# try 10 "-10+20-0+(-20)+(+20);"
# try 1 "4 == 4;"
# try 0 "4 == 5;"
# try 1 "5 != 98;"
# try 1 "4 < 5;"
# try 1 "3 <= 6;"
# try 1 "6 > 2;"
# try 1 "9 >= 3;"
# try 1 "(32+4*+4-3) > ((12-90+32));"
# try 5 "a = 5;"
# try 17 "a = 4 + (3*3) + 8/2;"
# try 106 "a = 106; a;"
# try 14 "a = 10; b = 4; a+b;"
# try 35 "a = 10; b = 30; c = 5; a+b-c;"
# try 5 "return 5;"
# try 10 "return a = 4+6;"
# try 19 "a = 5; b = 4; c = 10; d = 100; return a+b+c; 100;"
# try 1 "if(1) 1;"
# try 10 "if (0) 1; else 10;"
# try 5 "if (1) 5; else 10;"
# try 20 "a = 10; b = 10; if (a == b) 20; else 3;"
# try 3 "a = 10; b = 11; if (a == b) 20; else 3;"
# try 15 "iff = 10; elsee = 5; return iff+elsee;"
# try 10 "if (1) 20; return 10;"
# try 3 "a = 10; while(a-3) a = a-1; return a;"
# try 10 "a = 10; b = 20; while(0) a = a-1; return a;"
# try 15 "for(a = 4; a < 15; a = a+1) a = a; return a;"
# try 25 "a = 10; for(; a < 25;a = a+1) a = a; return a;"
# try 14 "a = 5; for(;;) if(a > 13) return a; else a = a+1;"
# try 60 "{ a = 10; b = 20; c = 30; return a+b+c; } return 100;"
# try 33 "a = 10; if (a == 10) { b = 20; c = 3; a = a+b+c; return a;}"
# try 55 "sum = 0; for(cnt = 0; cnt <= 10; cnt = cnt+1){ sum = sum + cnt; } return sum;"
# try 12 "a = 0; if (1 == 2) return 2; else a = 5; if (1 == 3) return 6; else a = a+7; if (2 == 5) return 129; else return a;"
# try 12 "hoge(a, b){ return a+b; } return hoge(5, 7);"
try 10 "main(){ return 10; }"
try 20 "main(){ a = 5; b = 4; c = 11; return a+b+c; }"
try 10 "hoge(){ return 10; } main(){ return hoge(); }"
# try 7 "hoge(a, b) { return a+b; } main(){ return hoge(3, 4); }"

echo -e "\n\e[32m=== try test SUCCESS ===\e[m\n"

echo -e "\e[32m=== compile_err test start ===\e[m\n"

compile_err "return 4;"
# compile_err "return j;;;"
# compile_err "for(a = 0;a < 4;;) if(1) return 5;"
# compile_err "foo(1,4,);"
# compile_err "foo(,);"
# compile_err "foo());"
# compile_err "a = 5; {a = 6; b = 6;} return a+b;"

echo -e "\n\e[32m=== compile_err test SUCCESS ===\e[m"

