CC=gcc
CFLAGS= -Wall -g -std=c99 -D _BSD_SOURCE

all: server client

server: server.c
	$(CC) $(CFLAGS) -o $@ $< -lpthread

client: client.c
	$(CC) $(CFLAGS) -o $@ $<


zip: sso6.zip

sso6.zip: *.c makefile
	zip $@ $^



