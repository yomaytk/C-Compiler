CFLAGS=-std=c11 -g -static

msscc: msscc.c parse.c
	gcc -o msscc msscc.c parse.c

test: msscc
	./test.sh

clean:
	rm -f msscc *.o *~ tmp*

.PHONY: test clean