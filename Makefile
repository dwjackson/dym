EXE_NAME = dym
SRC_FILES = dym.c edist.c
OBJ_FILES = $(SRC_FILES:.c=.o)
MAN_PAGE = dym.1
CC = cc
CFLAGS = -g -O0 -Wall -Wextra -pedantic
PREFIX=/usr/local

all: $(EXE_NAME)

$(EXE_NAME): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $(EXE_NAME) $(OBJ_FILES)

$(OBJ_FILES): $(SRC_FILES)
	$(CC) $(CFLAGS) -c $(SRC_FILES)

check: test_edist $(EXE_NAME)
	./test_edist
	./test_dym.sh

test_edist: test_edist.c edist.c
	$(CC) $(CFLAGS) -o test_edist test_edist.c edist.c -lastrounit

$(MAN_PAGE).gz: $(MAN_PAGE)
	gzip -c $(MAN_PAGE) > $(MAN_PAGE).gz

install: $(EXE_NAME) $(MAN_PAGE).gz
	mkdir -p $(DISTDIR)$(PREFIX)/bin
	mkdir -p $(DISTDIR)$(PREFIX)/share/man/man1
	install $(EXE_NAME) $(DISTDIR)$(PREFIX)/bin
	install $(MAN_PAGE).gz $(DISTSIR)$(PREFIX)/share/man/man1

uninstall:
	rm $(DISTDIR)$(PREFIX)/bin/$(EXE_NAME)
	rm $(DISTSIR)$(PREFIX)/share/man/man1/$(MAN_PAGE).gz

clean:
	rm -f $(EXE_NAME)
	rm -f test_edist
	rm -f *.o
	rm -f $(MAN_PAGE).gz

.PHONY: all clean test install uninstall
