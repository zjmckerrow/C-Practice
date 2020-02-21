CC	= gcc
CFLAGS 	= -Wall -Wextra -std=c99 -g
OBJFILE = da.o cda.o stack.o queue.o amaze.o maze.o cell.o
TARGET 	= amaze

all : $(TARGET)

$(TARGET) : $(OBJFILE)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILE)

da.o : da.c da.h
	$(CC) $(CFLAGS) -c da.c

cda.o : cda.c cda.h
	$(CC) $(CFLAGS) -c cda.c

stack.o : stack.c stack.h
	$(CC) $(CFLAGS) -c stack.c

queue.o : queue.c queue.h
	$(CC) $(CFLAGS) -c queue.c

maze.o : maze.c maze.h
	$(CC) $(CFLAGS) -c maze.c

cell.o : cell.c cell.h
	$(CC) $(CFLAGS) -c cell.c

amaze.o : maze.c maze.h amaze.c
	$(CC) $(CFLAGS) -c amaze.c

test :
	./amaze -r 44 -c 10 20 m -s m p -d p

valgrind : $(TARGET)
	valgrind ./amaze

clean :
	rm -f $(OBJFILE) $(TARGET)

