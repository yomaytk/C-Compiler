CFLAGS=-std=c11 -g -static

msscc: main.c parse.c codegen.c
	gcc -o msscc main.c parse.c codegen.c

test: msscc
	./test.sh

clean:
	rm -f msscc *.o *~ tmp*

.PHONY: test clean