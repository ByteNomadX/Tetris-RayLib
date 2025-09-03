CC = gcc
FLAGS = -Wall -Wextra
SRC = main
EXEC = tetris
DEBUG_FLAGS = -g -O0

H_DIR = ./headers

LIBS := $(shell pkg-config --libs raylib)
INCLUDES := $(shell pkg-config --cflags raylib)

ifeq ($(DEBUG),1) 
	FLAGS := $(FLAGS) $(DEBUG_FLAGS)
endif

$(EXEC): $(SRC).o ui.o figure.o field.o bag.o audio.o
	$(CC) $(FLAGS) $(SRC).c ui.o bag.o figure.o field.o audio.o -o $(EXEC) $(INCLUDES) $(LIBS)

$(SRC).o: $(SRC).c
	$(CC) $(FLAGS) -c $(SRC).c $(INCLUDES)

ui.o: ui.c
	$(CC) $(FLAGS) -c ui.c $(INCLUDES)

figure.o: figure.c 
	$(CC) $(FLAGS) -c figure.c $(INCLUDES)

field.o: field.c 
	$(CC) $(FLAGS) -c field.c $(INCLUDES)

bag.o: bag.c
	$(CC) $(FLAGS) -c bag.c $(INCLUDES)

audio.o: audio.c
	$(CC) $(FLAGS) -c audio.c $(INCLUDES)

clean:
	rm -rf *.o