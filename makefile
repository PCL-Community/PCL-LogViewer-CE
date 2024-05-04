CC=gcc
CFLAGS=-Wall -Wextra -DWIN32
LDFLAGS=-mwindows

all: Main.exe

Main.exe: Main.o resource.o
	$(CC) $(CFLAGS) -o Main.exe Main.o resource.o $(LDFLAGS)

Main.o: Main.c
	$(CC) $(CFLAGS) -c Main.c

resource.o: resource.rc
	windres resource.rc -o resource.o

clean:
	del Main.exe Main.o resource.o