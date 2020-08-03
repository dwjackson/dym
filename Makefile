EXE_NAME = dym
MAN_PAGE = dym.1
SRC_FILES = dym.c edist.c lowercase.c closest.c
OBJ_FILES = $(SRC_FILES:.c=.o)
CC = cc
CFLAGS = -O2 -Wall -Wextra -pedantic
PREFIX=/usr/local

all: $(EXE_NAME)

$(EXE_NAME): dym.o edist.o lowercase.o closest.o
	$(CC) $(CFLAGS) -o $@ $(OBJ_FILES) 

closest.o: closest.c dym.h
	$(CC) $(CFLAGS) -c $<

dym.o: dym.c dym.h config.h lowercase.h
	$(CC) $(CFLAGS) -c $<

edist.o: edist.c dym.h
	$(CC) $(CFLAGS) -c $<

lowercase.o: lowercase.c lowercase.h
	$(CC) $(CFLAGS) -c $<

check: test_edist $(EXE_NAME)
	./test_edist
	./test_dym.sh

test_edist: test_edist.c edist.c dym.h
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
