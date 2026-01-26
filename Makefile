CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -g

SRCS = src/xorl.c main.c

TARGET = program

all:
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)
clean:
	rm -f $(TARGET)
