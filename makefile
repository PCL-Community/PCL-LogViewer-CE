Main.exe: Main.o
	gcc -o Main.exe Main.o

Main.o: Main.c
	gcc -c -o Main.o Main.c

.PHONY: cleanall
cleanall:
	rm -f Main.exe Main.o