# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra

# Target executable
TARGET = dh_genkey

# Source files
SRCS = dh_genkey.c \
       dh_fct.c \
	   ../beacons/dh_bal.c

# Build target
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Clean generated files
clean:
	rm -f $(TARGET)

# Rebuild from scratch
re: clean $(TARGET)

.PHONY: clean re