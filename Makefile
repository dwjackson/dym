EXE_NAME = dym
SRC_FILES = dym.c edist.c lowercase.c
OBJ_FILES = $(SRC_FILES:.c=.o)
MAN_PAGE = dym.1
CC = cc
CFLAGS = -O2 -Wall -Wextra -pedantic
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
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	mkdir -p $(DESTDIR)$(PREFIX)/share/man/man1
	install $(EXE_NAME) $(DESTDIR)$(PREFIX)/bin
	install $(MAN_PAGE).gz $(DESTDIR)$(PREFIX)/share/man/man1

uninstall:
	rm $(DESTDIR)$(PREFIX)/bin/$(EXE_NAME)
	rm $(DESTSIR)$(PREFIX)/share/man/man1/$(MAN_PAGE).gz

clean:
	rm -f $(EXE_NAME)
	rm -f test_edist
	rm -f *.o
	rm -f $(MAN_PAGE).gz

.PHONY: all clean test install uninstall
