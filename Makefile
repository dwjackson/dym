EXE_NAME = dym
LIB_NAME = lib$(EXE_NAME)
MAN_PAGE = dym.1
LIB_MAN_PAGE = libdym.3
SRC_FILES = dym.c edist.c lowercase.c closest.c
OBJ_FILES = $(SRC_FILES:.c=.o)
CC = cc
CFLAGS = -O2 -Wall -Wextra -pedantic
PREFIX=/usr/local

all: $(EXE_NAME)

$(EXE_NAME): dym.o lowercase.o $(LIB_NAME).a
	$(CC) -L. $(CFLAGS) -o $@ $(OBJ_FILES) -ldym

$(LIB_NAME).a: edist.o closest.o
	ar rcs $(LIB_NAME).a edist.o closest.o

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

$(LIB_MAN_PAGE).gz: $(LIB_MAN_PAGE)
	gzip -c $(LIB_MAN_PAGE) > $(LIB_MAN_PAGE).gz

install: $(EXE_NAME) $(MAN_PAGE).gz $(LIB_MAN_PAGE).gz
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	mkdir -p $(DESTDIR)$(PREFIX)/share/man/man1
	mkdir -p $(DESTDIR)$(PREFIX)/share/man/man3
	mkdir -p $(DESTDIR)$(PREFIX)/lib
	mkdir -p $(DESTDIR)$(PREFIX)/include
	install $(EXE_NAME) $(DESTDIR)$(PREFIX)/bin
	install $(MAN_PAGE).gz $(DESTDIR)$(PREFIX)/share/man/man1
	install $(LIB_MAN_PAGE).gz $(DESTDIR)$(PREFIX)/share/man/man3
	install $(LIB_NAME).a $(DESTDIR)$(PREFIX)/lib
	install dym.h $(DESTDIR)$(PREFIX)/include

uninstall:
	rm $(DESTDIR)$(PREFIX)/bin/$(EXE_NAME)
	rm $(DESTDIR)$(PREFIX)/share/man/man1/$(MAN_PAGE).gz
	rm $(DESTDIR)$(PREFIX)/share/man/man3/$(LIB_MAN_PAGE).gz
	rm $(DESTDIR)$(PREFIX)/lib/$(LIB_NAME).a
	rm $(DESTDIR)$(PREFIX)/include/dym.h

clean:
	rm -f $(EXE_NAME)
	rm -f test_edist
	rm -f *.o
	rm -f $(MAN_PAGE).gz
	rm -f $(LIB_MAN_PAGE).gz
	rm -f *.a

.PHONY: all clean test install uninstall
