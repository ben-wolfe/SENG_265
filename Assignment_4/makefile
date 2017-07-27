# Makefile to to implement phase2.c

CC=gcc
CFLAGS=-c -Wall -g -ansi -DDEBUG

all: phase2

phase2: phase2.o linkedlist.o
	$(CC) phase2.o linkedlist.o -o phase2

phase2.o: phase2.c linkedlist.h
	$(CC) $(CFLAGS) phase2.c

linkedlist.o: linkedlist.c linkedlist.h
	$(CC) $(CFLAGS) linkedlist.c

clean:
	rm -rf *.o phase2 testlist
