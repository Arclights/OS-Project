CC		= gcc

CFLAGS		= -g -Wall

LDFLAGS		= -g

OUT		= test

OBJS		= test.o malloc.o

all: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(OUT) -g
	./test
check: all
clean:
	rm -f *.o test
