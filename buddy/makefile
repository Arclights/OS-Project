CC		= gcc

CFLAGS		= -g -Wall

LDFLAGS		= -g

OUT		= test2

OBJS		= test2.o malloc.o

all: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(OUT)
	./test2 10
check: all
clean:
	rm -f *.o test2