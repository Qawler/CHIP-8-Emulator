CXX ?= g++

CXXFLAGS ?= --std=c++17 -Wall -Werror -pedantic -g -Wno-sign-compare -Wno-comment

LDLIBS += -lSDL3

main.exe: main.cpp memory.cpp
	$(CXX) $(CXXFLAGS) $^ $(LDLIBS) -o $@