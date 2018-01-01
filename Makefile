TARGET = tetris
CC = gcc

OBJS = main.o

all : $(TARGET)
	
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

%.o : %.c
	$(CC) -c $< -o $@
