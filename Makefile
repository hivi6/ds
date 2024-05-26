CC     := gcc
CFLAGS := -Wall -Wextra

BUILD_DIR := build

EXAMPLE_DIR   := examples
EXAMPLE_FILES := $(wildcard $(EXAMPLE_DIR)/*.c)
EXAMPLE_EXES  := $(EXAMPLE_FILES:$(EXAMPLE_DIR)/%.c=$(BUILD_DIR)/%)

all: $(EXAMPLE_EXES)

$(EXAMPLE_BUILD)/%: $(EXAMPLE_DIR)/%.c | $(BUILD_DIR)

$(BUILD_DIR)/%: $(EXAMPLE_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I. -o $@ $^

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
