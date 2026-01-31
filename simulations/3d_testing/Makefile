CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I../../lib/SDL/include
LDFLAGS = -L../../lib/SDL/lib -lSDL2 -lm

TARGET = 3d_testing

SRC = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: all
	./$(TARGET)
