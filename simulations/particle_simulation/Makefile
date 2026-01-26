CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I../../lib/SDL/include
LDFLAGS = -L../../lib/SDL/lib -lSDL2 -lm

TARGET = particle_simulation

SRC = particle_simulation.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: all
	./$(TARGET)
