CFLAGS=-std=c11 -g -static

msscc: msscc.c
	gcc -o msscc msscc.c

test: msscc
	./test.sh

clean:
	rm -f msscc *.o *~ tmp*

.PHONY: test clean