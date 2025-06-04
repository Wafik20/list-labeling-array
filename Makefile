CC = gcc
CFLAGS = -Wall -Wextra -g

# Source files
SRC = lla.c main.c

# Object files
OBJ = lla.o main.o

# Output target
TARGET = program

.PHONY: all clean

# Default build target
all: $(TARGET)

# Link object files into final executable
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

# Compile .c files to .o files
%.o: %.c lla.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJ) $(TARGET)
