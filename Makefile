# Variables
CC = gcc
CFLAGS = -Wall -g
TARGET = compiler

# Source and object files
SOURCES = compiler.c symbol-table.c ll1parser.c
OBJECTS = $(SOURCES:.c=.o)

# Default target
all: $(TARGET)

# Link object files to create the final executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up the build
clean:
	rm -f *.o $(TARGET)

# Run the program after building
run: all
	./$(TARGET)

.PHONY: all clean run
