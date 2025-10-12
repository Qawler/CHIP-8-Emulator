#include "memory.hpp"


#include <iostream>
#include <vector>


int main(){

    Ram memory(4096);

    byte lsb = 0x1F;
    byte msb = 0xFF;

    address out = bytestoAddress(lsb, msb);

    std::cout << std::hex << static_cast<int>(out) << std::endl;

    return 0;
}