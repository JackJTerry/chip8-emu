CC           := gcc
SRC_DIR      := ./src
INC_DIR      := ./include
BUILD_DIR    := ./build
SRCS         := $(wildcard $(SRC_DIR)/*.c)
OBJS         := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
CFLAGS       :=  -I$(INC_DIR) -D_REENTRANT -std=c99 -Wall -Wextra -Werror
LDFLAGS      :=  -lSDL2main -lSDL2

PROGRAM      := $(BUILD_DIR)/chip8.x86

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $(PROGRAM)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)