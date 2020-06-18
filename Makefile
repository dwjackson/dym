EXE_NAME = dym
SRC_FILES = dym.c edist.c
OBJ_FILES = $(SRC_FILES:.c=.o)
CC = cc
CFLAGS = -g -O0 -Wall -Wextra -pedantic

all: $(EXE_NAME)

$(EXE_NAME): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $(EXE_NAME) $(OBJ_FILES)

$(OBJ_FILES): $(SRC_FILES)
	$(CC) $(CFLAGS) -c $(SRC_FILES)

check: test_edist
	./test_edist

test_edist: test_edist.c edist.c
	$(CC) $(CFLAGS) -o test_edist test_edist.c edist.c -lastrounit

clean:
	rm -f $(EXE_NAME)
	rm -f test_edist
	rm -f *.o

.PHONY: all clean test
