#====================================
# @file   : Makefile
# @version: 2020-03-13
# @created: 2020-02-26
# @brief  : makefile for gengalaxy
#====================================

TARGET   = gengalaxy
SHELL    = /bin/bash
CXX      = /usr/bin/g++
# debug
CXXFLAGS = -Wall -O2 -std=c++17 -Ilib -Iext
#release
#CXXFLAGS = -Wall -O4 -std=c++17 -Ilib -Iext

OBJ = gengalaxy.o
BIN = gengalaxy

all:	$(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDLIBS)-o $(BIN) $(OBJ) $(LDFLAGS)
	strip $(BIN)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN) $(OBJ) *~

