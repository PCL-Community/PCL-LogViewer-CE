CC=gcc
CFLAGS=-Wall -Wextra -std=c99

all: Main

Main: Main.o
	$(CC) $(CFLAGS) -o Main Main.o

Main.o: Main.c
	$(CC) $(CFLAGS) -c Main.c

clean:
	rm -f Main Main.o