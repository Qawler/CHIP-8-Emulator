#include "memory.hpp"

#include <SDL3/SDL.h>
#include <stack>



void setFont(Ram* ram){

    //Declaring pixel values for each hexdecmial value to be stored in memory

    std::vector<std::vector<byte>> font;

    std::vector<byte> zero = {0xF0, 0x90, 0x90, 0x90, 0xF0}; //0
    font.insert(font.end(), zero);

    std::vector<byte> one = {0x20, 0x60, 0x20, 0x20, 0x70}; //1
    font.insert(font.end(), one);

    std::vector<byte> two = {0xF0, 0x10, 0xF0, 0x80, 0xF0}; //2
    font.insert(font.end(), two);

    std::vector<byte> three = {0xF0, 0x10, 0xF0, 0x10, 0xF0}; //3
    font.insert(font.end(), three);

    std::vector<byte> four = {0x90, 0x90, 0xF0, 0x10, 0x10}; //4
    font.insert(font.end(), four);

    std::vector<byte> five = {0xF0, 0x80, 0xF0, 0x10, 0xF0}; //5
    font.insert(font.end(), five);

    std::vector<byte> six = {0xF0, 0x80, 0xF0, 0x90, 0xF0}; //6
    font.insert(font.end(), six);

    std::vector<byte> seven = {0xF0, 0x10, 0x20, 0x40, 0x40}; //7
    font.insert(font.end(), seven);

    std::vector<byte> eight = {0xF0, 0x90, 0xF0, 0x90, 0xF0}; //8
    font.insert(font.end(), zero);

    std::vector<byte> nine = {0xF0, 0x90, 0xF0, 0x10, 0xF0}; //9
    font.insert(font.end(), nine);

    std::vector<byte> A = {0xF0, 0x90, 0xF0, 0x90, 0x90}; //A
    font.insert(font.end(), A);

    std::vector<byte> B = {0xE0, 0x90, 0xE0, 0x90, 0xE0}; //B
    font.insert(font.end(), B);

    std::vector<byte> C = {0xF0, 0x80, 0x80, 0x80, 0xF0}; //C
    font.insert(font.end(), C);

    std::vector<byte> D = {0xE0, 0x90, 0x90, 0x90, 0xE0}; //D
    font.insert(font.end(), D);

    std::vector<byte> E = {0xF0, 0x80, 0xF0, 0x80, 0xF0}; //E
    font.insert(font.end(), E);

    std::vector<byte> F = {0xF0, 0x80, 0xF0, 0x80, 0x80}; //F
    font.insert(font.end(), F);

    address memory_address = 0x050; //Sticking with standard font location

    for (int i = 0; i < font.size(); ++i){
        for (int j = 0; j < font[i].size(); ++j){
            ram->write_address(memory_address, font[i][j]);
            memory_address += 1;
        }
    }
    

}



int main(){
    Ram memory(4096);

    std::stack<address> st;

    setFont(&memory);

    
    
    
    return 0;
}
