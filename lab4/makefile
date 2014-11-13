CC=gcc
CFLAGS= -Wall -g -std=c99 -D _BSD_SOURCE

all: thread 

thread: thread.c
	$(CC) $(CFLAGS) -o $@ $< -lpthread


zip: sso4.zip

sso4.zip: *.c makefile
	zip $@ $^



