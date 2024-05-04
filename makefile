Main.exe: Main.o
	gcc Main.o -o Main.exe

Main.o: Main.c
	gcc -c Main.c -o Main.o

.PHONY: cleanall
cleanall:
	rm -f Main.o