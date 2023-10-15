CC = gcc
CFLAGS = -Wall -std=c99 -O3
TARGET = main
OBJS = main.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f $(OBJS) $(TARGET)
