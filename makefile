main:Main.o
	gcc main.o -o main

main.o:main.c
	gcc -c main.c -o main.o

.PHONY:
clean_all:
	rm main.o
