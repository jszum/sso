CC=gcc
CFLAGS= -Wall -g -D_SVID_SOURCE -DEAPI -DPTHREAD_PROCESS_SHARED

all: komitet-barrier

komitet-barrier: komitet-barrier.c
	$(CC) $(CFLAGS) komitet-barrier.c -o $@ -lpthread


