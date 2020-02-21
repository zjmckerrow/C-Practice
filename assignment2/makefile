CC	= gcc
CFLAGS 	= -Wall -Wextra -std=c99 -g
OBJFILE = cda.o queue.o tree.o tnode.o bst.o gst.o rbt.o string.o scanner.o integer.o
TARGET 	= trees

all : $(TARGET)

$(TARGET) : $(OBJFILE)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILE)

cda.o : cda.c cda.h
	$(CC) $(CFLAGS) -c cda.c

queue.o : queue.c queue.h
	$(CC) $(CFLAGS) -c queue.c

tnode.o : tnode.c tnode.h
	$(CC) $(CFLAGS) -c tnode.c

bst.o : bst.c bst.h
	$(CC) $(CFLAGS) -c bst.c

gst.o : gst.c gst.h
	$(CC) $(CFLAGS) -c gst.c

rbt.o : rbt.c rbt.h
	$(CC) $(CFLAGS) -c rbt.c

string.o : string.c string.h
	$(CC) $(CFLAGS) -c string.c

integer.o : integer.c integer.h
	$(CC) $(CFLAGS) -c integer.c

scanner.o : scanner.c scanner.h
	$(CC) $(CFLAGS) -c scanner.c

tree.o : gst.c gst.h rbt.c rbt.h string.c string.h tree.c scanner.c scanner.h integer.c integer.h
	$(CC) $(CFLAGS) -c tree.c

test :
	./trees data commands

valgrind :
	valgrind ./trees

clean :
	rm -f $(OBJFILE) $(TARGET)
