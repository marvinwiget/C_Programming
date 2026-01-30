# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Wextra -O3
LDFLAGS = -lm

# Target Executable Name
TARGET = nn_app

# Source and Object Files
SRCS = main.c matrix.c neuralnetwork.c
OBJS = $(SRCS:.c=.o)
HDRS = activation.h matrix.h neuralnetwork.h

# Default Rule
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Compile Source Files to Object Files
%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

# Run the program
run: all
	./$(TARGET)

.PHONY: all clean run
