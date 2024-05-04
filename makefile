Main.exe:main.o
	gcc Main.c -o main.o

main.o:Main.c
	gcc -c main.o -o Main.exe

.PHONY:
cleanall:
	rm main.o
