TAR = Main.c
OBJ = main.o
CC := gcc
RMRF := rm 

$(TAR):$(OBJ)
	$(CC) $^ -o $@
%.o:%.c
	$(CC) -c $^ -o $@

.PHONY:
cleanall:
	$(RMRF) $(OBJ)
