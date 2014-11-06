CC=gcc
CFLAGS= -Wall -g -std=c99

all: thread 

thread: thread.c
	$(CC) $(CFLAGS) -o $@ $< -lpthread


zip: sso4.zip

sso4.zip: *.c makefile
	zip $@ $^



