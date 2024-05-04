TAR = Main.c
OBJ = main.o
CC := gcc
RMRF := rm 

$(TAR):$(OBJ)
	$(CC) $^ -o $@
%.o:%.c
	$(CC) -c Main.c -o $@

.PHONY:
cleanall:
	$(RMRF) $(OBJ)
