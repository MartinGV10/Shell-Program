# Makefile to build rush from rush.c

CC      := gcc
CFLAGS  := -std=c11 -Wall -Wextra -Wpedantic -O2
TARGET  := rush
SRC     := rush.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) *.o
