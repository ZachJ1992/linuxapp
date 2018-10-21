EXE = FCPROCESS
OBJ = fcprocess.o
SRC = fcprocess.c

CC = gcc
CFLAGS = -g
#LDFLAGS = -L . –lFOO #链接目录

EXE:$(OBJ)
	$(CC) $(LDFLAGS) -o $(EXE) $^

OBJ:$(SRC)
	#$(CC) $(CFLAGS) -o $(OBJ) -c $^
	$(CC) -o $(OBJ) -c $^

#%.o:%.c
#	$(CC) -c $(CFLAGS) $< -o $@

.PHONY:clean
clean:
	-$(RM) $(OBJ) $(EXE)
