TARGET = tetris
CC = gcc

OBJS = main.o \
	game.o

all : $(TARGET)
	
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) #-lncurces

%.o : %.c
	$(CC) -c $< -o $@

clean :
	rm *.o $(TARGET)
