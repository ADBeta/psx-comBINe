# Output directories
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

# TARGET
TARGET := $(BIN_DIR)/psx-combine

# Source files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
# Objects derived from Sources
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Compiler
CC      := g++
WIN32CC := i686-w64-mingw32-g++
WIN64CC := x86_64-w64-mingw32-g++

# Flags for Linux
CPPFLAGS  := -Iinclude -O2 -MMD -MP -Wall -Wextra -Wsign-conversion -Wmissing-declarations -Wconversion -Wshadow -Wlogical-op -Wfloat-equal -Wunused -Wuninitialized -Wformat -Wunused-result -Wtype-limits
CFLAGS    := -Wall -std=c++17 `wx-config --cxxflags`
LDLIBS    := -lm `wx-config --libs`

# Flags for Windows
WIN32CFLAGS := -O2 -Wall -static-libgcc -static-libstdc++ `/usr/i686-w64-mingw32/bin/wx-config --cxxflags`
WIN64CFLAGS := -O2 -Wall -static-libgcc -static-libstdc++ `/usr/x86_64-w64-mingw32/bin/wx-config --cxxflags`

WIN32LDLIBS := -lm -static `/usr/i686-w64-mingw32/bin/wx-config --libs`
WIN64LDLIBS := -lm -static `/usr/x86_64-w64-mingw32/bin/wx-config --libs`

.PHONY: all linux win32 win64 install clean

# Default target
all: linux

# Linux target (dynamic linking)
linux: $(TARGET)

# Windows 32-bit target (static linking)
win32:
	$(MAKE) CC=$(WIN32CC) CFLAGS="$(WIN32CFLAGS)" LDLIBS="$(WIN32LDLIBS)" $(TARGET)

# Windows 64-bit target (static linking)
win64:
	$(MAKE) CC=$(WIN64CC) CFLAGS="$(WIN64CFLAGS)" LDLIBS="$(WIN64LDLIBS)" $(TARGET)

# Make binary
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

# Make objects
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@ 

# Create obj and bin directory if they don't exist
$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

install: $(TARGET)
	mv ./$(TARGET) /usr/local/bin/
	
# Remove objects and binary
clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)

