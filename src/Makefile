CFLAGS=-std=c11 -g -static
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

mss9cc: $(OBJS)
	$(CC) -Wall -g -o mss9cc $(OBJS) $(LDFLAGS)

$(OBJS): mss9cc.h

test: mss9cc
	./test.sh

clean:
	rm -f mss9cc *.o *~ tmp*

.PHONY: test clean