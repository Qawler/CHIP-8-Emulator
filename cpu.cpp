#include "cpu.hpp"
#include <iomanip>


address pc = 0x0; //Program counter

address index_reg = 0x0; //Index counter

std::array<byte, 16> var_regs = {0}; //Variable registers

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

    for (int i = 0; i < buffer.size() + 1; ++i){
        memory->write_address(pc, buffer[i]);
        ++pc;  
    }

    pc = 0x200;
}

opcode fetch(Ram *memory){
    opcode op;
    op.MSB = memory->read_address(pc);
    ++pc;
    op.LSB = memory->read_address(pc);
    ++pc;

    op.apply_bitmask();

    return op;   
}

void print_data(){
    std::cout << "Program Counter: " << std::hex << pc << std::endl;
    std::cout << "Index Register: " << std::hex << index_reg << std::endl;
    std::cout << "V Registers: [";
    for (int i = 0; i < 15; ++i){
        std::cout << var_regs[i] << ", "; 
    }
    std::cout << var_regs[15] << "]" << std::endl; 
}

void decode_and_excute(Ram *memory, std::vector<std::vector<bool>> &video_memory, std::map<SDL_Scancode, bool> &keys, byte &delay_timer, byte &sound_timer){
    opcode current_instruction = fetch(memory);

    std::cout << "Running Op: " << std::hex << current_instruction << std::endl;

    if ((current_instruction.MSB == 0x00) && (current_instruction.LSB == 0x00)){
        return ;
    }

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
            six_instr(current_instruction);
            break;
        case 0x7:
            seven_instr(current_instruction);
            break;
        case 0x8:
            break;
        case 0x9:
            break;
        case 0xa:
            a_instr(current_instruction);
            break;
        case 0xb:
            break;
        case 0xc:
            break;
        case 0xd:
            d_instr(current_instruction, memory, video_memory);
            break;
        case 0xe:
            break;
        case 0xf:
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
            for (int i = 0; i < 32; ++i){
                for (int j = 0; j < 64; ++j){
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

void a_instr(opcode &op){
    index_reg = op.NNN;
}

void d_instr(opcode &op, Ram *memory, std::vector<std::vector<bool>> &video_memory){
    byte x = var_regs[op.second_nibble] % 64;
    byte y = var_regs[op.third_nibble] % 32;
    byte n = op.fourth_nibble;

    //std::cout << "Draw Op" << std::endl;

    for (int i = 0; i < n; ++i){ //Iterate through height of sprite
        byte sprite = memory->read_address(index_reg + i);
        if ((y + i) > 32){
            break;
        }
        for (int j = 7; j >= 0; --j){ //Iterate through each bit in sprite
            int bit_value = (sprite >> j) & 1;  
            if (x + j > 64){
                continue;
            }
            else if (bit_value && (video_memory[y + i][x + j])){
                video_memory[y + i][x + j] = false;
                var_regs[0xF] = 1;
            }
            else if (bit_value && (!video_memory[y + i][x + j])){
                video_memory[y + i][x + j] = true; 
            }
            //std::cout << "Sprite: " << sprite << ", Mask: " << bit_value << ", VX: "<< x << ", VY: " << y << ", (X, Y): (" << x + j << ", " << y + i << "), Pixel Value: " << video_memory[y + i][x + j] << std::endl;
        }    
    }
    
}
