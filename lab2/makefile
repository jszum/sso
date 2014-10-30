CC=gcc
CFLAGS= -Wall -g -std=c99

all: zad1 zad1b zad2 zad2b player

zad1: zad1.o

zad1.o : zad1.c
	$(CC) $(CFLAGS) -c -o $@ $<

zad1b: zad1b.o

zad1b.o : zad1b.c
	$(CC) $(CFLAGS) -c -o $@ $<

zad2: zad2.o

zad2.o : zad2.c
	$(CC) $(CFLAGS) -c -o $@ $<

zad2b: zad2b.o

zad2b.o : zad2b.c
	$(CC) $(CFLAGS) -c -o $@ $<

player: player.o

player.o : player.c
	$(CC) $(CFLAGS) -c -o $@ $<

zip: sso2.zip

sso2.zip: *.c makefile komentarz
	zip $@ $^



