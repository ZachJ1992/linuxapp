EXE = MAIN1
OBJ = loadprogram.o ../rfifo/rfifo.o ../fifosrc/fifo.o
SRC = loadprogram.c ../rfifo/rfifo.c ../fifosrc/fifo.c

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
