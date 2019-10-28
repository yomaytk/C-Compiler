#!/bin/bash
try() {
	expected="$1"
	input="$2"

	./msscc "$input" > tmp.s
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

try 0 0
try 42 42
try 20 "8+12"
try 21 "5+20-4"
try 41 " 12 + 34 - 5 "
try 179 "78 + 6-32+ 6 + 58-62 +125"
try 47 '5+6*7'
try 15 '5*(9-6)'
try 4 '(3+5)/2'
try 179 "(((78))+6-32+ 6 +((58-62)+125)*1/1*2/2)"
try 10 "-(+20-30-0)"
try 10 "-10+20-0+(-20)+(+20)"
try 1 "4 == 4"
try 0 "4 == 5"
try 1 "5 != 98"
try 1 "4 < 5"
try 1 "3 <= 6"
try 1 "6 > 2"
try 1 "9 >= 3"
try 1 "(32+4*+4-3) > ((12-90+32))"

echo -e "\e[32mOK\e[m"