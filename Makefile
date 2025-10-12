CXX ?= g++

CXXFLAGS ?= --std=c++17 -Wall -Werror -pedantic -g -Wno-sign-compare -Wno-comment

main.exe: main.cpp memory.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@