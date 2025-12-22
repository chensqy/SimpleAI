CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall
SRC := src/main.cpp src/util.cpp src/responder.cpp src/engine.cpp
INC := -Isrc
BIN := chat

all: $(BIN)
$(BIN): $(SRC)
$(CXX) $(CXXFLAGS) $(INC) -o $@ $(SRC)
clean:
rm -f $(BIN)
