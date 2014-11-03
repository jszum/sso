CC=gcc
CFLAGS= -Wall -g -std=c99 -D_SVID_SOURCE

all: philosopher manager

philosopher: philosopher.o

philosopher.o : philosopher.c
	$(CC) $(CFLAGS) -c -o $@ $<

manager: manager.o

manager.o : manager.c
	$(CC) $(CFLAGS) -c -o $@ $<

zip: sso3.zip

sso3.zip: *.c makefile komentarz run
	zip $@ $^



