TAR = Main.c
OBJ = main.o
CC := gcc

Main.c:main.o
	gcc Main.c -o main.o

main.o:Main.c
	gcc -c main.o -o Main.c

.PHONY:
cleanall:
	rm main.o
