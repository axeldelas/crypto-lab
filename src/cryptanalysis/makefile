# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra

# Target executable
TARGET = dh_crack

# Source files
SRCS = ../beacons/dh_bal.c \
       dh_crack.c \
       dh_crack_c1.c \
       dh_crack_c2.c

# Build target
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Clean generated files
clean:
	rm -f $(TARGET)

# Rebuild from scratch
re: clean $(TARGET)

.PHONY: clean re
