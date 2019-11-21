#!/bin/bash
try() {
	expected="$1"
	input="$2"

	./mss9cc "$input" > tmp.s
	gcc -o tmp tmp.s
	./tmp
	actual="$?"

	if [ "$actual" = "$expected" ]; then
		echo "$input => $actual"
	else
		echo -e "\e[31mFAILED\e[m"
		echo "$input => $expected expected, but got $actual"
		exit 1
	fi
}

try 0 "0;"
try 42 "42;"
try 20 "8+12;"
try 21 "5+20-4;"
try 41 " 12 + 34 - 5; "
try 179 "78 + 6-32+ 6 + 58-62 +125;"
try 47 '5+6*7;'
try 15 '5*(9-6);'
try 4 '(3+5)/2;'
try 179 "(((78))+6-32+ 6 +((58-62)+125)*1/1*2/2);"
try 10 "-(+20-30-0);"
try 10 "-10+20-0+(-20)+(+20);"
try 1 "4 == 4;"
try 0 "4 == 5;"
try 1 "5 != 98;"
try 1 "4 < 5;"
try 1 "3 <= 6;"
try 1 "6 > 2;"
try 1 "9 >= 3;"
try 1 "(32+4*+4-3) > ((12-90+32));"
try 5 "a = 5;"
try 17 "a = 4 + (3*3) + 8/2;"
try 106 "a = 106; a;"
try 14 "a = 10; b = 4; a+b;"
try 35 "a = 10; b = 30; c = 5; a+b-c;"
try 5 "return 5;"
try 10 "return a = 4+6;"
try 19 "a = 5; b = 4; c = 10; d = 100; return a+b+c; 100;"
try 1 "if(1) 1;"
try 10 "if (0) 1; else 10;"
try 5 "if (1) 5; else 10;"
try 20 "a = 10; b = 10; if (a == b) 20; else 3;"
try 3 "a = 10; b = 11; if (a == b) 20; else 3;"
try 15 "iff = 10; elsee = 5; return iff+elsee;"
try 10 "if (1) 20; return 10;"
try 3 "a = 10; while(a-3) a = a-1; return a;"
try 10 "a = 10; b = 20; while(0) a = a-1; return a;"
try 15 "for(a = 4; a < 15; a = a+1) a = a; return a;"
try 25 "a = 10; for(; a < 25;a = a+1) a = a; return a;"
try 14 "a = 5; for(;;) if(a > 13) return a; else a = a+1;"

echo -e "\e[32mOK\e[m"