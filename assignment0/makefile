CC	= gcc
CFLAGS	= -Wall -Wextra -std=c99 -g
LDFLAGS = 
OBJFILE	= da.o cda.o stack.o queue.o cdatest2.o integer.o
TARGET	= assignment0

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

cdatest2.o : cda.c cda.h
	$(CC) $(CFLAGS) -c cdatest2.c

integer.o : integer.c integer.h
	$(CC) $(CFLAGS) -c integer.c

test : 
	./assignment0 5000

valgrind : $(TARGET)
		valgrind ./assignment0

clean : 
	rm -f $(OBJFILE) $(TARGET)
