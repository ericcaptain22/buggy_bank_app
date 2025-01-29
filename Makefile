CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0` -Wall -g
LDFLAGS = `pkg-config --libs gtk+-3.0`
SRC = src/main.c src/bugs.c
BUILD_DIR = build
TARGET = $(BUILD_DIR)/bank_app

all: $(TARGET)

$(TARGET): $(SRC)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
