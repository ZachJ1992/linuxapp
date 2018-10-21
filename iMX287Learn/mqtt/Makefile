EXE = appmain
OBJ = appmain.o src/mqtt/sdk/mqtt.o src/mqtt/sdk/cJSON.o src/mqtt/sdk/mqtt_buffer.o src/mqtt/mqtt_api.o
SRC = appmain.c src/mqtt/sdk/mqtt.c src/mqtt/sdk/cJSON.c src/mqtt/sdk/mqtt_buffer.c src/mqtt/mqtt_api.c

CC = gcc
#CC = arm-none-linux-gnueabi-gcc
CFLAGS = -g
#LDFLAGS = -L . –lpthread #链接目录
LDFLAGS +=-pthread
LINKFLAGS +=-lrt -lm #链接库:-lrt对应shm_open(); -lm对应pow()

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

