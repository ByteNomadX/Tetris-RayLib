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

ui.o: ui.c $(H_DIR)/structs.h $(H_DIR)/constants.h
	$(CC) $(FLAGS) -c ui.c $(INCLUDES)

figure.o: figure.c $(H_DIR)/structs.h $(H_DIR)/constants.h $(H_DIR)/enums.h
	$(CC) $(FLAGS) -c figure.c $(INCLUDES)

field.o: field.c $(H_DIR)/structs.h $(H_DIR)/constants.h
	$(CC) $(FLAGS) -c field.c $(INCLUDES)

clean:
	rm -rf *.o

