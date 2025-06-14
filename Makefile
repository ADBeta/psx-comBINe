# Output directories
SRC_DIR := src
BIN_DIR := bin
OBJ_DIR := obj
INC_DIR := include

LINUX_OBJ_DIR := obj/linux
WIN32_OBJ_DIR := obj/win32
WIN64_OBJ_DIR := obj/win64

# Target binaries
LINUX_BIN := $(BIN_DIR)/linux/psx-combine
WIN32_BIN := $(BIN_DIR)/win32/psx-combine.exe
WIN64_BIN := $(BIN_DIR)/win64/psx-combine.exe

# Source files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)

# Object files
LINUX_OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(LINUX_OBJ_DIR)/%.o, $(SRCS))
WIN32_OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(WIN32_OBJ_DIR)/%.o, $(SRCS))
WIN64_OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(WIN64_OBJ_DIR)/%.o, $(SRCS)) $(WIN64_RC)

# Compilers
LINUX_CC := g++
WIN32_CC := i686-w64-mingw32-g++
WIN64_CC := x86_64-w64-mingw32-g++

# windres Program for compiling the resource file
WIN32_WR := i686-w64-mingw32-windres
WIN64_WR := x86_64-w64-mingw32-windres


###
# Linux Compile and Linker Flags
LINUX_CFLAGS    := -I$(INC_DIR) -O2 -Wall -std=c++17 $(shell wx-config --cxxflags)
LINUX_LDLIBS    := -lm $(shell wx-config --libs)


###
# Windows Resource file
WIN_RC_FILE := $(SRC_DIR)/resources.rc
# Windows Resrouce File Object Target
WIN32_RC    := $(WIN32_OBJ_DIR)/resources.o
WIN64_RC    := $(WIN64_OBJ_DIR)/resources.o

# Windows WxWidgets Paths --- Change if needed
WIN64_WXCONFIG := /opt/wxwidgets-win64/bin/wx-config



# Windows 64-bit wxWidgets Compiler/Linker/windres flags
WIN64_CFLAGS := -I$(INC_DIR) -O2 -Wall -static-libgcc -static-libstdc++ $(shell $(WIN64_WXCONFIG) --cxxflags) -DUNICODE -D_UNICODE
WIN64_LDLIBS := -lm -static $(shell $(WIN64_WXCONFIG) --libs)
WIN64_RCFLAG := $(shell $(WIN64_WXCONFIG) --cxxflags) # Can be --rcflags on some systems

# Phony targets
.PHONY: all linux win32 win64 clean

# Default target: linux
all: linux

# Linux build
linux: $(LINUX_BIN)

$(LINUX_BIN): $(LINUX_OBJS) | $(BIN_DIR)/linux
	$(LINUX_CC) $^ $(LINUX_LDLIBS) -o $@

$(LINUX_OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(LINUX_OBJ_DIR)
	$(LINUX_CC) $(LINUX_CFLAGS) -c $< -o $@







# Windows 64-bit build
win64: $(WIN64_BIN)

$(WIN64_BIN): $(WIN64_OBJS) $(WIN64_RC) | $(BIN_DIR)/win64
	$(WIN64_CC) $^ $(WIN64_LDLIBS) -o $@

$(WIN64_OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(WIN64_OBJ_DIR)
	$(WIN64_CC) $(WIN64_CFLAGS) -c $< -o $@

$(WIN64_RC): $(WIN_RC_FILE) | $(WIN64_OBJ_DIR)
	$(WIN64_WR) -I$(SRC_DIR) $(WIN64_RCFLAG) $< -O coff -o $@





# Create directories
$(BIN_DIR)/linux $(BIN_DIR)/win32 $(BIN_DIR)/win64:
	mkdir -p $@

$(LINUX_OBJ_DIR) $(WIN32_OBJ_DIR) $(WIN64_OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

