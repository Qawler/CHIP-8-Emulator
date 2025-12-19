#include "cpu.hpp"

#include <iomanip>


address pc = 0x0; //Program counter

address index_reg = 0x0; //Index counter

std::array<byte, 16> var_regs = {0}; //Variable registers

std::stack<address> program_stack;

std::random_device rd;

std::mt19937 engine(rd());

std::uniform_int_distribution<int> dist(0, 255);

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

void print_data(byte &delay_timer, byte &sound_timer){
    std::cout << "Program Counter: " << std::hex << pc << std::endl;
    std::cout << "Index Register: " << std::hex << index_reg << std::endl;
    std::cout << "V Registers: [";
    for (int i = 0; i < 15; ++i){
        std::cout << var_regs[i] << ", "; 
    }
    std::cout << var_regs[15] << "]" << std::endl; 
    std::cout << "Delay Timer: " << delay_timer << std::endl;
    std::cout << "Sound Timer: " << sound_timer << std::endl;
}

void decode_and_excute(Ram *memory, std::vector<std::vector<bool>> &video_memory, std::map<SDL_Scancode, bool> &keys, byte &delay_timer, byte &sound_timer, bool debug_mode){
    opcode current_instruction = fetch(memory);

    if (debug_mode){
        std::cout << "Running Op: " << std::hex << current_instruction << std::endl;
    }

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
            two_instr(current_instruction);
            break;
        case 0x3:
            three_instr(current_instruction);
            break;
        case 0x4:
            four_instr(current_instruction);
            break;
        case 0x5:
            five_instr(current_instruction);
            break;
        case 0x6:
            six_instr(current_instruction);
            break;
        case 0x7:
            seven_instr(current_instruction);
            break;
        case 0x8:
            eight_instr(current_instruction);
            break;
        case 0x9:
            nine_instr(current_instruction);
            break;
        case 0xa:
            a_instr(current_instruction);
            break;
        case 0xb:
            b_instr(current_instruction);
            break;
        case 0xc:
            c_instr(current_instruction);
            break;
        case 0xd:
            d_instr(current_instruction, memory, video_memory);
            break;
        case 0xe:
            e_instr(current_instruction, keys);
            break;
        case 0xf:
            f_instr(current_instruction, keys, delay_timer, sound_timer, memory);
            break;  
    }
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

void two_instr(opcode &op){
    program_stack.push(pc);
    pc = op.NNN;
}

void three_instr(opcode &op){
    if (var_regs[op.second_nibble] == op.LSB){
        pc += 2;
    }
}

void four_instr(opcode &op){
    if (var_regs[op.second_nibble] != op.LSB){
        pc += 2;
    }
}

void five_instr(opcode &op){
    if (var_regs[op.second_nibble] == var_regs[op.third_nibble]){
        pc += 2;
    }
}

void six_instr(opcode &op){
    var_regs[op.second_nibble] = op.LSB;
}

void seven_instr(opcode &op){
    var_regs[op.second_nibble] += op.LSB;
}

void eight_instr(opcode &op){
    byte carry;
    switch(op.fourth_nibble){
        case (0x0):
            var_regs[op.second_nibble] = var_regs[op.third_nibble];
            break;
        case (0x1):
            var_regs[op.second_nibble] = var_regs[op.second_nibble] | var_regs[op.third_nibble];
            break;
        case (0x2):
            var_regs[op.second_nibble] = var_regs[op.second_nibble] & var_regs[op.third_nibble];
            break;
        case (0x3):
            var_regs[op.second_nibble] = var_regs[op.second_nibble] ^ var_regs[op.third_nibble];
            break;
        case (0x4):
            carry = (var_regs[op.second_nibble] + var_regs[op.third_nibble] > 0xFF) ? 0x01 : 0x00;
            var_regs[op.second_nibble] = var_regs[op.second_nibble] + var_regs[op.third_nibble];
            var_regs[0xF] = carry;
            break;
        case (0x5):
            carry = (var_regs[op.second_nibble] >= var_regs[op.third_nibble]) ? 0x01 : 0x00;
            var_regs[op.second_nibble] = var_regs[op.second_nibble] - var_regs[op.third_nibble]; 
            var_regs[0xF] = carry;
            break;
        case (0x6):
            carry = var_regs[op.second_nibble] & 0x01;
            var_regs[op.second_nibble] = var_regs[op.second_nibble] >> 1;
            var_regs[0xF] = carry;
            break;
        case (0x7):
            carry = (var_regs[op.third_nibble] >= var_regs[op.second_nibble]) ? 0x01 : 0x00;
            var_regs[op.second_nibble] = var_regs[op.third_nibble] - var_regs[op.second_nibble]; 
            var_regs[0xF] = carry;
            break;
        case (0xE):
            carry = (var_regs[op.second_nibble] & 0x80) >> 7;
            var_regs[op.second_nibble] = var_regs[op.second_nibble] << 1;
            var_regs[0xF] = carry;
    }
}

void nine_instr(opcode &op){
    if (var_regs[op.second_nibble] != var_regs[op.third_nibble]){
        pc += 2;
    }
}

void a_instr(opcode &op){
    index_reg = op.NNN;
}

void b_instr(opcode &op){
    pc = op.NNN + var_regs[0];
}

void c_instr(opcode &op){
    int random_int = dist(engine); 
    var_regs[op.second_nibble] = op.LSB & random_int;
}

void d_instr(opcode &op, Ram *memory, std::vector<std::vector<bool>> &video_memory){
    byte x = var_regs[op.second_nibble] % 64;
    byte y = var_regs[op.third_nibble] % 32;
    byte n = op.fourth_nibble;


    for (int i = 0; i < n; ++i){ //Iterate through height of sprite
        byte sprite = memory->read_address(index_reg + i);
        if ((y + i) > 32){
            break;
        }
        for (int j = 7; j >= 0; --j){ //Iterate through each bit in sprite
            int bit_value = (sprite >> j) & 1;  
            if ((x + (7 - j)) > 64){
                continue;
            }
            if (video_memory[y + i][x + (7 - j)] && bit_value){
                var_regs[0xF] = 0x01;
            }
            video_memory[y + i][x + (7 - j)] = video_memory[y + i][x + (7 - j)] ^ bit_value;
        }   
    }
    
}

void e_instr(opcode &op, std::map<SDL_Scancode, bool> &keys){
    switch (op.LSB){
        case 0x9E:
            if (check_key_press(var_regs[op.second_nibble], keys)){
                pc += 2;
            }
            break;
        case 0xA1:
            if (!check_key_press(var_regs[op.second_nibble], keys)){
                pc += 2;
            }
            break;
    }
}

void f_instr(opcode &op, std::map<SDL_Scancode, bool> &keys, byte &delay_timer, byte &sound_timer, Ram *memory){
    address pc_value = pc;
    address font_value = 0x050;
    std::map<SDL_Scancode, bool> original_key_press = keys;
    byte num;
    switch (op.LSB){
        case 0x07:
            var_regs[op.second_nibble] = delay_timer;
            break;
        case 0x15:
            delay_timer = var_regs[op.second_nibble];
            break;
        case 0x18:
            sound_timer = var_regs[op.second_nibble];
            break;
        case 0x1E:
            //var_regs[0xF] = ((index_reg + var_regs[op.second_nibble]) > 0x0FFF) ? 0x01 : 0x00;
            index_reg += var_regs[op.second_nibble];
            break;
        case 0x0A:
            
            while (check_valid_keys_changed(keys, original_key_press)){
                pc = pc_value;
            }
            for (byte i = 0x0; i <= 0xF; ++i){
                if (check_key_press(i, keys) && check_key_press(i, original_key_press)){
                    continue;
                }
                else {
                    var_regs[op.second_nibble] = i;
                }
            } 
            break;
        case 0x29:
            index_reg = font_value + (5 * var_regs[op.second_nibble]);
            break;
        case 0x33:
            num = var_regs[op.second_nibble];
            memory->write_address(index_reg, std::floor((num / 100) % 10));
            memory->write_address(index_reg + 1, std::floor((num / 10) % 10));
            memory->write_address(index_reg + 2, num % 10);
            break;
        case 0x55:
            for (byte i = 0x0; i <= op.second_nibble; ++i){
                memory->write_address(index_reg + i, var_regs[i]);
            }
            break;
        case 0x65:
            for (byte i = 0x0; i <= op.second_nibble; ++i){
                var_regs[i] = memory->read_address(index_reg + i);
            }
            break;
    }
}

bool check_key_press(byte value, std::map<SDL_Scancode, bool> &keys){
    switch(value){
        case 0x00:
            return keys[SDL_SCANCODE_0];
        case 0x01:
            return keys[SDL_SCANCODE_1];
        case 0x02:
            return keys[SDL_SCANCODE_2];
        case 0x03:
            return keys[SDL_SCANCODE_3];
        case 0x04:
            return keys[SDL_SCANCODE_Q];
        case 0x05:
            return keys[SDL_SCANCODE_W];
        case 0x06:
            return keys[SDL_SCANCODE_E];
        case 0x07:
            return keys[SDL_SCANCODE_A];
        case 0x08:
            return keys[SDL_SCANCODE_S];
        case 0x09:
            return keys[SDL_SCANCODE_D];
        case 0x0A:
            return keys[SDL_SCANCODE_Z];
        case 0x0B:
            return keys[SDL_SCANCODE_C];
        case 0x0C:
            return keys[SDL_SCANCODE_4];
        case 0x0D:
            return keys[SDL_SCANCODE_R];
        case 0x0E:
            return keys[SDL_SCANCODE_F];
        case 0x0F:
            return keys[SDL_SCANCODE_V];
    }
    return false;
}

bool check_valid_keys_changed(std::map<SDL_Scancode, bool> &keys, std::map<SDL_Scancode, bool> &original_keys){
    bool all_same = true;
    for (byte i = 0x0; i <= 0xF; ++i){
        if (check_key_press(i, keys) && check_key_press(i, original_keys)){
            continue;
        }
        else {
            all_same = false;
        }
    }
    return all_same;
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