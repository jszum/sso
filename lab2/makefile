CC=gcc
CFLAGS= -Wall -g -std=c99

all: zad1 zad1b

zad1: zad1.o

zad1.o : zad1.c
	$(CC) $(CFLAGS) -c -o $@ $<

zad1b: zad1b.o

zad1b.o : zad1b.c
	$(CC) $(CFLAGS) -c -o $@ $<

zip: sso2.zip

sso1.zip: *.c makefile
	zip $@ $^



