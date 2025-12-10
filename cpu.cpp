#include "cpu.hpp"

int pc = 0x0; //Program counter

int index = 0x0; //Index register "I"

int delay_timer = 0x0; //Decreases at 60 Hz

int sound_timer = 0x0; //Gives off a beep at 0 

std::array<int, 16> var_regs = {0};

std::vector<opcode> program_instructions;


void read_file(std::ifstream &fin){
    std::string code;
    while (fin >> code){
        
    }
}

opcode fetch(){

}

void decode_and_excute(Ram *memory, std::vector<std::vector<bool>> &video_memory){

}