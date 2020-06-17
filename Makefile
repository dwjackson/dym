EXE_NAME = dym
SRC_FILES = dym.c
OBJ_FILES = $(SRC_FILES:.c=.o)
CC = cc
CFLAGS = -g -O0 -Wall -Wextra -pedantic

all: $(EXE_NAME)

$(EXE_NAME): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $(EXE_NAME) $(OBJ_FILES)

$(OBJ_FILES): $(SRC_FILES)
	$(CC) $(CFLAGS) -c $(SRC_FILES)

clean:
	rm -f $(EXE_NAME)
	rm -f *.o

.PHONY: all clean
