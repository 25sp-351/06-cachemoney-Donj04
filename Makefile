
OBJS = inputmanager.o converter.o cache.o
LIB = libmemoize.so

TARGET = main
CACHE_TESTER = testcache
OUTPUT_TESTER = tests.sh

CC = gcc
CFLAGS = -g -Wall -Wextra


all: $(TARGET) $(CACHE_TESTER) $(LIB)


run_main: $(TARGET) $(LIB)
	./$(TARGET) ./$(LIB)


run_cache_test: $(CACHE_TESTER) $(LIB)
	./$(CACHE_TESTER) ./$(LIB)


run_output_test:
	./$(OUTPUT_TESTER)


lib%.so: %.c cache.h
	$(CC) -shared -fPIC $(CFLAGS) -o $@ $<


$(TARGET): $(TARGET).o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^


$(CACHE_TESTER): $(CACHE_TESTER).o $(OBJS)
	$(CC) -o $@ $(CFLAGS) $(CACHE_TESTER).o $(OBJS) -lbsd


$(TARGET).o: inputmanager.h converter.h cache.h

$(CACHE_TESTER).o: inputmanager.h converter.h cache.h

inputmanager.o: inputmanager.c inputmanager.h

converter.o: converter.c converter.h inputmanager.h


clean:
	rm -f $(TARGET) $(CACHE_TESTER) $(TARGET).o $(CACHE_TESTER).o $(OBJS) $(LIB)
