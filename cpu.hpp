#ifndef CPU_HPP
#define CPU_HPP

#include <cstdint>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <array>
#include <stack>

#include "memory.hpp"

typedef uint8_t byte;

struct opcode {
    private:
        byte front_mask = 0xF0;
        byte back_mask = 0x0F;

    public: 

        byte MSB = 0x00;
        byte LSB = 0x00;
    

        byte first_nibble = (MSB & front_mask) >> 4;
        byte second_nibble = (MSB & back_mask);
        byte third_nibble = (LSB & front_mask) >> 4;
        byte fourth_nibble = (LSB & back_mask);
};

void read_file(std::ifstream &fin);

opcode fetch();

void decode_and_excute(Ram *memory, std::vector<std::vector<bool>> &video_memory);

#endif