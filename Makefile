CC=g++
CFLAGS=-c -Wall
LDFLAGS=
BUILDDIR=build
DOCDIR=doc
SOURCES=$(wildcard src/*.cxx) $(wildcard src/matrices/*.cxx)
EXECUTABLE=morozan1
OBJECTS=$(SOURCES:.cxx=.o)

run: $(EXECUTABLE)
		./$(EXECUTABLE)

valgrind: $(EXECUTABLE)
		valgrind --leak-check=full --show-leak-kinds=all ./$(EXECUTABLE)

compile: $(SOURCES) $(EXECUTABLE)
		rm -rf $(BUILDDIR)
		mkdir $(BUILDDIR)
		mv $(OBJECTS) $(BUILDDIR)

all: $(SOURCES) $(OBJECTS)
		rm -rf $(BUILDDIR)
		mkdir $(BUILDDIR)
		mv $(OBJECTS) $(BUILDDIR)

doc:
		rm -rf $(DOCDIR)
		mkdir $(DOCDIR)
		doxygen Doxyfile

clean:
		rm -f  $(OBJECTS)
		rm -rf $(BUILDDIR)
		rm -rf $(DOCDIR)
		rm -f  $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
		$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o: %.cxx
		rm -f $@
		$(CC) $(CFLAGS) $< -o $@
