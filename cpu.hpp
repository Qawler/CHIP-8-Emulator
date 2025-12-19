#ifndef CPU_HPP
#define CPU_HPP

#include <cstdint>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <array>
#include <stack>
#include <map>
#include <random>
#include <cmath>

#include <SDL3/SDL_scancode.h>

#include "memory.hpp"

typedef uint8_t byte;

struct opcode {
    private:
        byte front_mask = 0xf0;
        byte back_mask = 0x0f;
        byte add_zero = 0x00;


    public: 

        byte MSB = 0x00;
        byte LSB = 0x00;
    
        byte first_nibble = 0x00;
        byte second_nibble = 0x00;
        byte third_nibble = 0x00;
        byte fourth_nibble = 0x00;

        address NNN = 0x00;
 

        void apply_bitmask(){
            first_nibble = (MSB & front_mask) >> 4;
            second_nibble = (MSB & back_mask);
            third_nibble = (LSB & front_mask) >> 4;
            fourth_nibble = (LSB & back_mask);

            NNN = (second_nibble << 8) | (third_nibble << 4) | fourth_nibble;
        }
};

void read_file(std::ifstream &fin, Ram *memory);

opcode fetch(Ram *memory);

void print_data(byte &delay_timer, byte &sound_timer);

void decode_and_excute(Ram *memory, std::vector<std::vector<bool>> &video_memory, std::map<SDL_Scancode, bool> &keys, byte &delay_timer, byte &sound_timer, bool debug_mode);

bool check_key_press(byte value, std::map<SDL_Scancode, bool> &keys);

bool check_valid_keys_changed(std::map<SDL_Scancode, bool> &keys, std::map<SDL_Scancode, bool> &original_keys);

std::ostream & operator<<(std::ostream &os, const opcode &data);

std::ostream & operator<<(std::ostream &os, const std::vector<opcode> &data);

void zero_instr(opcode &op, Ram *memory, std::vector<std::vector<bool>> &video_memory);

void one_instr(opcode &op);

void two_instr(opcode &op);

void three_instr(opcode &op);

void four_instr(opcode &op);

void five_instr(opcode &op);

void six_instr(opcode &op);

void seven_instr(opcode &op);

void eight_instr(opcode &op);

void nine_instr(opcode &op);

void a_instr(opcode &op);

void b_instr(opcode &op);

void c_instr(opcode &op);

void d_instr(opcode &op, Ram *memory, std::vector<std::vector<bool>> & video_memory);

void e_instr(opcode &op, std::map<SDL_Scancode, bool> &keys);

void f_instr(opcode &op, std::map<SDL_Scancode, bool> &keys, byte &delay_timer, byte &sound_timer, Ram *memory);

#endif