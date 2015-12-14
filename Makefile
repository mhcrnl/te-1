CSTD = c90

CFLAGS = -g -W -Wall -std=$(CSTD) -pedantic
LDFLAGS = -lncurses

SOURCES = $(wildcard *.c)
OBJECTS = $(notdir $(SOURCES:.c=.o))
EXECUTABLE = te

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	rm $(OBJECTS)

$(OBJECTS): $(SOURCES)
	$(CC) $(CFLAGS) -c $(SOURCES)

clean:
	rm $(OBJECTS) $(EXECUTABLE)
