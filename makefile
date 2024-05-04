CC=gcc
CFLAGS=-Wall -Wextra -DWIN32
LDFLAGS=-mwindows

all: Main.exe

Main.exe: Main.o
	$(CC) $(CFLAGS) -o Main.exe Main.o $(LDFLAGS)

Main.o: Main.c
	$(CC) $(CFLAGS) -c Main.c

clean:
	del Main.exe Main.o