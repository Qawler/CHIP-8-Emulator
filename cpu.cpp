#include "cpu.hpp"
#include <iomanip>

address pc = 0x0; //Program counter

address index = 0x0; //Index counter

std::array<int, 16> var_regs = {0}; //Variable registers

std::stack<address> program_stack;

void read_file(std::ifstream &fin, Ram *memory){
    size_t end = fin.tellg(); //Getting position of end of file 
    fin.seekg(0, std::ios::beg); //Moving to the beginning of file
    size_t beg = fin.tellg();
    const size_t len = end - beg; //Determine size of file

    std::vector<byte> buffer(len);

    fin.seekg(0, std::ios::beg);
    fin.read(reinterpret_cast<char*>(buffer.data()), len);
    fin.close();

    pc = 0x200;

    for (int i = 1; i < buffer.size() + 1; ++i){
        std::cout << buffer[i] << std::endl;
        memory->write_address(pc, buffer[i]);
        ++pc;  
    }

    pc = 0x200;
}

opcode fetch(Ram *memory){
    opcode op;
    op.MSB = memory->read_address(pc);
    std::cout << "MSB: " << op.MSB << std::endl;
    ++pc;
    op.LSB = memory->read_address(pc);
    std::cout << "LSB: " << op.LSB << std::endl;

    std::cout << op << std::endl;
    address NNN = op.NNN;
    std::cout << std::hex << NNN << std::endl;

    return op;   
}

void decode_and_excute(Ram *memory, std::vector<std::vector<bool>> &video_memory, std::vector<std::vector<bool>> &keys, byte &delay_timer, byte &sound_timer){
    opcode current_instruction = fetch(memory);

    switch (current_instruction.first_nibble){
        case 0x0:
            zero_instr(current_instruction, memory, video_memory);
            break;
        case 0x1:
            one_instr(current_instruction);
            break;
        case 0x2:
            break;
        case 0x3:
            break;
        case 0x4:
            break;
        case 0x5:
            break;
        case 0x6:
            break;
        case 0x7:
            break;
        case 0x8:
            break;
        case 0x9:
            break;
        case 0xA:
            break;
        case 0xB:
            break;
        case 0xC:
            break;
        case 0xD:
            break;
        case 0xE:
            break;
        case 0xF:
            break;  
    }
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

void zero_instr(opcode &op, Ram *memory, std::vector<std::vector<bool>> &video_memory){
    switch(op.fourth_nibble){
        case 0x0:
            for (int i = 0; i < video_memory.size(); ++i){
                for (int j = 0; j < video_memory[i].size(); ++j){
                    video_memory[i][j] = false;
                }
            }
            break; 
        
        case 0xE:
            address return_add = program_stack.top();
            program_stack.pop();
            pc = return_add;
            break;
    }
}

void one_instr(opcode &op){
    pc = op.NNN;
}

void six_instr(opcode &op){
    var_regs[op.second_nibble] = op.LSB;
}

void seven_instr(opcode &op){
    var_regs[op.second_nibble] += op.LSB;
}
