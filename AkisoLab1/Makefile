CC = gcc
CFLAGS = -Wall -Wextra -pedantic

APP_TARGET = myapp
TEST_TARGET = tests
SOURCES = Main.c MyGcd.h
TEST_SOURCES = MyTests.c
OBJECTS = $(SOURCES:.c=.o)
TEST_OBJECTS = $(TEST_SOURCES:.c=.o)

.PHONY: all clean tests

all: $(APP_TARGET)

$(APP_TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -o $<

clean:
	rm -f $(APP_TARGET) $(TEST_TARGET) $(TEST_OBJECTS)

tests: $(TEST_TARGET)

$(TEST_TARGET): $(TEST_SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -o $<
