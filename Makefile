CXX ?= g++

CXXFLAGS ?= --std=c++17 -Wall -Werror -pedantic -g -Wno-sign-compare -Wno-comment

LDLIBS += -lSDL3

chip8.exe: chip8.cpp memory.cpp cpu.cpp
	$(CXX) $(CXXFLAGS) $^ $(LDLIBS) -o $@