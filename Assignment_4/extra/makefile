# Ben Wolfe | V00205547
# 
# DESC:
# Makefile for single-link vs double-link comparison
#
# LAST MODIFIED:
# 28.07.2017

CC=gcc
CFLAGS=-c -Wall -g -DDEBUG

all: sll_phase2 dll_phase2

sll_phase2: sll_phase2.o sll.o
	$(CC) sll_phase2.o sll.o -o sll_phase2

dll_phase2: dll_phase2.o dll.o
	$(CC) dll_phase2.o dll.o -o dll_phase2

sll_phase2.o: sll_phase2.c sll.h
	$(CC) $(CFLAGS) sll_phase2.c

dll_phase2.o: dll_phase2.c dll.h
	$(CC) $(CFLAGS) dll_phase2.c

sll.o: sll.c sll.h
	$(CC) $(CFLAGS) sll.c

dll.o: dll.c dll.h
	$(CC) $(CFLAGS) dll.c

clean:
	rm -rf *.o sll_phase2 dll_phase2
