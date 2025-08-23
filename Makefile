CC = gcc
FLAGS = -Wall
SRC = main
EXEC = tetris

H_DIR = ./headers

LIBS := $(shell pkg-config --libs raylib)
INCLUDES := $(shell pkg-config --cflags raylib)

$(EXEC): $(SRC).o ui.o figure.o field.o
	$(CC) $(FLAGS) $(SRC).c ui.o figure.o field.o -o $(EXEC) $(INCLUDES) $(LIBS)

$(SRC).o: $(SRC).c
	$(CC) $(FLAGS) -c $(SRC).c $(INCLUDES)

ui.o: ui.c
	$(CC) $(FLAGS) -c ui.c $(INCLUDES)

figure.o: figure.c 
	$(CC) $(FLAGS) -c figure.c $(INCLUDES)

field.o: field.c 
	$(CC) $(FLAGS) -c field.c $(INCLUDES)

clean:
	rm -rf *.o