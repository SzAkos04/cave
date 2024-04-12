CC := clang
PROJECT := cave
CFLAGS := -Wall -Wextra -Werror -Wpedantic
# LLVM stuff
CFLAGS += `llvm-config --cflags`
LDFLAGS := `llvm-config --ldflags` -lm
LIBS := `llvm-config --libs`

INCLUDES := -Iinclude
SRC_DIR := src
SRC := $(wildcard $(SRC_DIR)/*.c)
BUILD_DIR := build
OBJ := $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Default target is 'debug'
.DEFAULT_GOAL := debug

.PHONY: all debug release clean

debug: CFLAGS += -Og
debug: | $(BUILD_DIR)/$(PROJECT)

release: CFLAGS += -O3 -march=native -mtune=native -fomit-frame-pointer -finline-functions
release: | $(BUILD_DIR)/$(PROJECT)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

$(BUILD_DIR)/$(PROJECT): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LIBS)

clean:
	rm -rf $(BUILD_DIR)

# Ensure directories exist before attempting to build
$(BUILD_DIR)/$(PROJECT): | $(BUILD_DIR)
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
