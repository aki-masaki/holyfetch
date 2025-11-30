CC = gcc
CFLAGS = -Wall -Wextra -O2 -Iinclude

SRC = $(wildcard src/*.c src/**/*.c)
OBJ = $(SRC:.c=.o)

TARGET = holyfetch

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ) $(TARGET)

run: $(TARGET)
	./$(TARGET)
