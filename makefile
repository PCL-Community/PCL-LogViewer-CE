CC=gcc
CFLAGS=-Wall -Wextra

all: Main.exe

Main.exe: Main.o
	$(CC) $(CFLAGS) -o Main.exe Main.o

Main.o: Main.c
	$(CC) $(CFLAGS) -c Main.c -DWIN32

clean:
	del Main.exe Main.o