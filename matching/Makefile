SOURCES = matching.c heap.c unionfind.c
HEADERS = matching.h heap.h unionfind.h
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))

CFLAGS  += -ansi -pedantic -Wall -g -Iglpk/include -Lglpk/lib
LDFLAGS += -lglpk -Wl,-rpath,glpk/lib

all: matching

matching: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o matching

-include .depends

.depends: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -MM $(SOURCES) > .depends

clean:
	/bin/rm -f $(OBJECTS) .depends matching.dat

purge: clean
	/bin/rm -f matching
