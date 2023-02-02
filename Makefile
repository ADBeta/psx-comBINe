#Output directories
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

#TARGET
TARGET := $(BIN_DIR)/psx-comBINe

#Source files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
#Objects derived from Sources
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

#Compiler
CC := g++
WINCC := x86_64-w64-mingw32-c++

#Flags
CPPFLAGS := -Iinclude -MMD -MP #NOTE. Include path is to /usr/include. this may be arch specific
CFLAGS   := -Wall
LDFLAGS  := -Llib -lboost_system -lboost_filesystem
LDLIBS   := -lm 

.PHONY: all clean

all: $(TARGET)

#Make binary
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

#Make objects
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@ 

#Create obj and bin directory if they don't exist
$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

#Windows compilation
win: $(OBJS) | $(BIN_DIR)
	$(WINCC) $(LDFLAGS) $^ $(LDLIBS) -o $@
#Remove objects and binary
clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)

-include $(OBJ:.o=.d)
