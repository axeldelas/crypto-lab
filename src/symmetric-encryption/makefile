# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra

# Target executable
TARGET = dh_crypt

# Source files
SRCS = dh_crypt.c \
       dh_crypt_xor.c \
       dh_crypt_cbc_crypt.c \
       dh_crypt_cbc_uncrypt.c \
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
