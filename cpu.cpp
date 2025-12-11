#include "cpu.hpp"
#include <iomanip>


int pc = 0x0; //Program counter

int index = 0x0; //Index register "I"

int delay_timer = 0x0; //Decreases at 60 Hz

int sound_timer = 0x0; //Gives off a beep at 0 

std::array<int, 16> var_regs = {0};

std::vector<opcode> program_instructions;


void read_file(std::ifstream &fin){
    size_t end = fin.tellg(); //Getting position of end of file 
    fin.seekg(0, std::ios::beg); //Moving to the beginning of file
    size_t beg = fin.tellg();
    const size_t len = end - beg; //Determine size of file

    std::vector<byte> buffer(len);

    fin.seekg(0, std::ios::beg);
    fin.read(reinterpret_cast<char*>(buffer.data()), len);
    fin.close();

    for (int i = 1; i < buffer.size() + 1; ++i){

        byte msb;
        byte lsb;

        if (i % 2 != 0){
            msb = buffer[i-1];
        }
        if (i % 2 == 0){
            lsb = buffer[i-1];
            opcode *new_instruction = new opcode();
            new_instruction->MSB = msb;
            new_instruction->LSB = lsb;
            program_instructions.push_back(*new_instruction);
        }
    }



}

//opcode fetch(){
    
//}

void decode_and_excute(Ram *memory, std::vector<std::vector<bool>> &video_memory){

}

std::ostream & operator<<(std::ostream &os, const opcode &data){
    return os << data.MSB << data.LSB;
}

std::ostream & operator<<(std::ostream &os, const std::vector<opcode> &data){
    for (int i = 0; i < data.size(); ++i){
        os << data[i] << " ";
        if ((i+1) % 8 == 0){
            os << std::endl;
        }
    }
    return os;
}