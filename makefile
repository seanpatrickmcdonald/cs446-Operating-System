#G++
CC=@g++

#Compiler Flags
CXXFLAGS=-Wall -std=c++11


#Current Directory
CUR_DIR = $(shell pwd)

#Include Directory
INC_DIR = $(CUR_DIR)/src

#Source Directory
SRC_DIR = $(CUR_DIR)/src

#Object Directory
OBJ_DIR = $(CUR_DIR)/src/objs

#Object Files
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))

#Include Paths
INCLUDES =-I $(SRC_DIR)

#Libraries
LIBRARIES = -pthread

#Program Name
PROG_NAME = Sim04


all: $(OBJ_FILES)
	$(CC) $(CXXFLAGS) -o $(PROG_NAME) sim04.cpp $(LIBRARIES) $(INCLUDES) $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CXXFLAGS) -c -o $@ $< $(LIBRARIES) $(INCLUDES)

clean:
	rm -rf $(PROG_NAME) $(OBJ_DIR)/*.o