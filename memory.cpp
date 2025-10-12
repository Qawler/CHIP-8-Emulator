#include "memory.hpp" 

Ram::Ram(){
    memory.push_back(zero);
    memory_size = 1;
}

Ram::Ram(int size){
    memory_size = size;
    for(int i = 0; i < size; i++){
        memory.push_back(zero);
    }
}

int Ram::get_memory_size(){
    return memory_size;
}

byte Ram::read_address(address address) const{
    return memory[address];
}

void Ram::write_address(address address, byte data){
    memory[address] = data;
}

void Ram::write_addresses(address start_address, address end_address, byte data){
    for (int i = start_address; i <= end_address; ++i){
        write_address(i, data);
    }
}

void Ram::zeroOut(address start_address, address end_address){
    write_addresses(start_address, end_address, zero);
}

void Ram::zeroOut(){
    zeroOut(0x00, memory_size);
}

Register::Register(){
    byte_data = 0;
    address_data = 0;
};

byte Register::read_byte(){
    return byte_data;
}

address Register::read_address(){
    return address_data;
}

void Register::write_byte(byte data){
    byte_data = data;
}

void Register::write_address(address data){
    address_data = data;
}

address bytestoAddress(byte lsb, byte msb){
    address lsb_push_back = lsb << 8;
    address lsb_and_msb = lsb_push_back | msb;
    address mask = 0x0FFF;
    address new_address = mask & lsb_and_msb;

    return new_address;
}

std::ostream & operator<<(std::ostream &os, const byte &data){
    return os << std::showbase << std::hex << static_cast<int>(data);
}

