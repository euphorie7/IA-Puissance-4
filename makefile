TARGET = proj_24

CC = gcc

CFLAGS = -Wall -g

SOURCES = proj_24.c

OBJECTS = $(SOURCES:.c=.o)

HEADER = proj_24.h

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean run

