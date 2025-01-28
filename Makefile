# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -pthread

# Targets
TARGET = chat_client
SRC = chat_client.c

# Default target
all: $(TARGET)

# Compile the target
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Clean up build files
clean:
	rm -f $(TARGET)

