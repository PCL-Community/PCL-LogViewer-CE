TAR = Main.c
OBJ = program1.o cube.o main.o
CC := gcc
RMRF := rm 

$(TAR):$(OBJ)
	$(CC) $^ -o $@
%.o:%.c
	$(CC) -c $^ -o $@

.PHONY:
cleanall:
	$(RMRF) $(OBJ)
