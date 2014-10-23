CC=gcc
CFLAGS= -Wall -g -std=c99

all: zad1 zad2 zad3 zad4 zad5

zad1: zad1.o

zad1.o : zad1.c
	$(CC) $(CFLAGS) -c -o $@ $<

zad2: zad2.o

zad2.o : zad2.c
	$(CC) $(CFLAGS) -c -o $@ $<

zad3: zad3.o

zad3.o : zad3.c
	$(CC) $(CFLAGS) -c -o $@ $<

zad4: zad4.o

zad4.o : zad4.c
	$(CC) $(CFLAGS) -c -o $@ $<

zad5 : zad5.o

zad5.o : zad5.c
	$(CC) $(CFLAGS) -c -o $@ $<

zip: sso1.zip

sso1.zip: *.c makefile
	zip $@ $^



