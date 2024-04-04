CC := gcc
PROJECT := cave
CFLAGS := -Wall -Wextra -Werror -Wpedantic
LDFLAGS :=
INCLUDES := -Iinclude
SRC_DIR := src
SRC := $(wildcard $(SRC_DIR)/*.c)
BUILD_DIR := build
OBJ := $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
.PHONY: all build run clean

all: build

build: $(BUILD_DIR)/$(PROJECT)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES) $(BUILD_ARGS)

$(BUILD_DIR)/$(PROJECT): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(BUILD_ARGS)

release:
	$(MAKE) BUILD_ARGS="-O3" -B

clean:
	rm -rf $(BUILD_DIR)
