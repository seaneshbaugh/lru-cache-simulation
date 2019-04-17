CC=clang
CFLAGS=-std=c17 -Wall -g

TARGET_NAME=lru-cache-simulation

SOURCE_FILES=\
	$(TARGET_NAME).c

INCLUDE_DIRECTORIES=

SYMBOLS=

all: clean default

default: $(TARGET_NAME)

$(TARGET_NAME):
	$(CC) $(CFLAGS) $(INCLUDE_DIRECTORIES) $(SYMBOLS) $(SOURCE_FILES) -o $(TARGET_NAME)

clean:
	rm -f $(TARGET_NAME)
	rm -rf $(TARGET_NAME).dSYM
