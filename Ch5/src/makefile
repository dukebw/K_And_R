CFLAGS=-c -Wall -std=c++11 -g #-O3
INCLUDE_FLAGS=
LDFLAGS=

CC=g++

SOURCES=dcl_parser.cpp util.cpp alloc.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=dcl_parser
# Build directory
BUILD = ../build/

all: directories $(SOURCES) $(BUILD)$(EXECUTABLE)

$(BUILD)$(EXECUTABLE): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< $(INCLUDE_FLAGS) -o $@

MKDIR_P = mkdir -p

.PHONY: directories OUT_DIR clean spotless

OUT_DIR = ../build/

directories: OUT_DIR

OUT_DIR:
	$(MKDIR_P) $(OUT_DIR)

clean:
	rm -f *.o

spotless:
	rm -f *.o *~ .*~ $(BUILD)$(EXECUTABLE)

util.o:alloc.h
dcl_parser.o:util.h
