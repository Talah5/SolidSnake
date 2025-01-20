CC = gcc
CFLAGS = -shared -fPIC -Wall -Wextra -ldl -Iinclude
SRC_DIR = src
BUILD_DIR = build

all: $(BUILD_DIR)/intercept.so

$(BUILD_DIR)/intercept.so: $(BUILD_DIR)/intercept.o $(BUILD_DIR)/exfiltration.o
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
