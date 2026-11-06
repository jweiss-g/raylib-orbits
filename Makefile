.DEFAULT_GOAL := all

CC        := clang
SRC_DIR   := src
OBJ_DIR   := build
BIN_DIR   := bin
TARGET    := $(BIN_DIR)/main

SRCS      := $(shell find $(SRC_DIR) -name '*.c')
OBJS      := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPS      := $(OBJS:.o=.d)

# Base flags (no platform paths here)
CFLAGS    := -std=c11 -Wall -Wextra -g -O0 -MMD -MP -I$(SRC_DIR)
LDFLAGS   :=
LDLIBS    :=

# Platform-specific flags
UNAME_S := $(shell uname -s)
UNAME_M := $(shell uname -m)

ifeq ($(OS),Windows_NT)
    CC      := gcc
    TARGET  := $(TARGET).exe
    LDLIBS  += -lraylib -lopengl32 -lgdi32 -lwinmm
else ifeq ($(UNAME_S),Darwin)
    ifeq ($(UNAME_M),arm64)
        BREW_PATH := /opt/homebrew
    else
        BREW_PATH := /usr/local
    endif
    CFLAGS  += -I$(BREW_PATH)/include
    LDFLAGS += -L$(BREW_PATH)/lib
    LDLIBS  += -lraylib -framework CoreVideo -framework IOKit \
               -framework Cocoa -framework OpenGL
else ifeq ($(UNAME_S),Linux)
    LDLIBS  += -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif

all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(OBJS) -o $@ $(LDFLAGS) $(LDLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR):
	@mkdir -p $@

clean:
	@rm -rf $(OBJ_DIR) $(BIN_DIR)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run

-include $(DEPS)