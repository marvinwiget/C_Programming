C = gcc
CFLAGS = -Wall -Wextra -I.. # -I.. tells the compiler to look in the parent dir for headers

# Paths
PARENT_DIR = ..
#
# # Target executable name
TARGET = mat_test
#
# # Source files
SRCS = mat_test.c $(PARENT_DIR)/matrix.c
# # Object files (replaces .c with .o)
OBJS = $(SRCS:.c=.o)
#
# # Default rule
all: $(TARGET)
#
# # Link the object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)
#
# 	# Rule for object files in the current directory
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
#
# 		# Rule for object files in the parent directory
$(PARENT_DIR)/%.o: $(PARENT_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
#
# 			# Clean up build files
clean:
	rm -f $(TARGET) $(OBJS)
#
.PHONY: all clean
