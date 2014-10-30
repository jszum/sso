sso3CC=gcc
CFLAGS= -Wall -g -std=c99

all: philosopher 

philosopher: philosopher.o

philosopher.o : philosopher.c
	$(CC) $(CFLAGS) -c -o $@ $<

zip: sso3.zip

sso3.zip: *.c makefile komentarz
	zip $@ $^



