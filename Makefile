CC := clang
PROJECT := cave
CFLAGS := -Wall -Wextra -Werror -Wpedantic
# LLVM stuff
CFLAGS += `llvm-config --cflags`
LDFLAGS := `llvm-config --ldflags`
LIBS := `llvm-config --libs`

INCLUDES := -Iinclude
SRC_DIR := src
SRC := $(wildcard $(SRC_DIR)/*.c)
BUILD_DIR := build
OBJ := $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
.PHONY: all build clean

all: build

build: $(BUILD_DIR)/$(PROJECT)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES) $(BUILD_ARGS)

$(BUILD_DIR)/$(PROJECT): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LIBS) $(BUILD_ARGS)

release:
	$(MAKE) BUILD_ARGS="-O3 -march=native -mtune=native -fomit-frame-pointer -ffast-math -finline-functions" -B

clean:
	rm -rf $(BUILD_DIR)
