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
        byte add_zero = 0x00;

    public: 

        byte MSB = 0x00;
        byte LSB = 0x00;
    

        byte first_nibble = (MSB & front_mask) >> 4;
        byte second_nibble = (MSB & back_mask);
        byte third_nibble = (LSB & front_mask) >> 4;
        byte fourth_nibble = (LSB & back_mask);

        address NNN = (add_zero << 24) | (second_nibble << 16) | (third_nibble << 8) | fourth_nibble;
};

void read_file(std::ifstream &fin, Ram *memory);

opcode fetch(Ram *memory);

void decode_and_excute(Ram *memory, std::vector<std::vector<bool>> &video_memory, std::vector<std::vector<bool>> &keys, byte &delay_timer, byte &sound_timer);

std::ostream & operator<<(std::ostream &os, const opcode &data);

std::ostream & operator<<(std::ostream &os, const std::vector<opcode> &data);

void zero_instr(opcode &op, Ram *memory, std::vector<std::vector<bool>> &video_memory);

void one_instr(opcode &op);

void six_instr(opcode &op);

void seven_instr(opcode &op);

#endif