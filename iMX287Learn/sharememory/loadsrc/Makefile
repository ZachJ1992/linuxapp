EXE = loadprogram
OBJ = loadprogram.o 
SRC = loadprogram.c 

CC = gcc
CFLAGS = -g
#LDFLAGS = -L . –lpthread #链接目录
LDFLAGS +=-pthread 
LINKFLAGS +=-lrt

EXE:$(OBJ)
	$(CC) $(LDFLAGS) -o $(EXE) $^ $(LINKFLAGS)

OBJ:$(SRC)
	$(CC) $(CFLAGS) -o $(OBJ) -c $^ $(LINKFLAGS)
	#$(CC) -o $(OBJ) -c $^

#%.o:%.c
#	$(CC) -c $(CFLAGS) $< -o $@

.PHONY:clean
clean:
	-$(RM) $(OBJ) $(EXE)
