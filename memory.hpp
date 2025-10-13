#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <vector>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <stack>
#include <string>

typedef uint8_t byte;
typedef uint16_t address;



class Ram {
    public:
        Ram();

        Ram(int size);

        int get_memory_size();
        byte read_address(address location) const;
        std::vector<byte> read_addresses(address start_location, address end_location);
        void write_address(address location, byte data);
        void write_addresses(address start_location, address end_location, byte data);   

        void zeroOut(address start_location, address end_location);
        void zeroOut();
        
    private:
        std::vector<byte> memory;
        int memory_size;
        byte zero = 0x00;

};

class Register{
    public:

    Register();

    byte read_byte();
    address read_address();
    void write_byte(byte data);
    void write_address(address data);

    private:
    byte byte_data;
    address address_data;

    
};

std::ostream & operator<<(std::ostream &os, const byte &data);

address bytestoAddress(byte lsb, byte msb);

#endif