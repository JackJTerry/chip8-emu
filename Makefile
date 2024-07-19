CC            := gcc
CXX           := g++

SRC_DIR       := ./src
INC_DIR       := ./include
BUILD_DIR     := ./build

IMGUI_DIR     := ./imgui
IMGUI_INC_DIR := ./include/imgui

SDL_INC_DIR   := /usr/include/SDL2

CFLAGS        :=  -I$(INC_DIR) -I$(IMGUI_INC_DIR) -D_REENTRANT -std=c99 -Wall -Wextra -Werror
CPPFLAGS      :=  -I$(INC_DIR) -I$(IMGUI_INC_DIR) -I$(SDL_INC_DIR) -D_REENTRANT -std=c++14 -Wall -Wextra
LDFLAGS       :=  -lSDL2main -lSDL2

C_SRCS        := $(wildcard $(SRC_DIR)/*.c)
C_OBJS        := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SRCS))
CPP_SRCS      := $(wildcard $(IMGUI_DIR)/*.cpp)
CPP_OBJS      := $(patsubst $(IMGUI_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(CPP_SRCS))

PROGRAM      := $(BUILD_DIR)/chip8.x86

all: $(PROGRAM)

$(PROGRAM): $(C_OBJS) $(CPP_OBJS)
	$(CXX) $(LDFLAGS) $^ -o $(PROGRAM)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(IMGUI_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)