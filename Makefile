SRCS = $(TARGET).c inputmanager.c converter.c cache.c
HDRS = inputmanager.h converter.h cache.h

TARGET = memoize

OBJS = $(SRCS:.c=.o)

CC = gcc
CFLAGS = -g -Wall -Wextra

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

inputmanager.o: inputmanager.c inputmanager.h

converter.o: converter.c converter.h

cache.o: cache.c cache.h

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJS) 
