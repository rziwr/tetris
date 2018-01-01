TARGET = tetris
CC = gcc

OBJS = main.o \
	game.o

all : $(TARGET)
	
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

%.o : %.c
	$(CC) -c $< -o $@

clean :
	rm *.o $(TARGET)
