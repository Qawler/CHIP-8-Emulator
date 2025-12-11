#include "memory.hpp"
#include "cpu.hpp"

#include <chrono>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

const int window_factor = 20;
const int WINDOW_WIDTH = 64 * window_factor;
const int WINDOW_HEIGHT = 32 * window_factor;

const double instruction_freq = 1.0 / 700; //Run at 700 Hz
const double timer_freq = 1.0 / 60; //Run at 60 Hz;

struct SDLState {
    SDL_Window *window;
    SDL_Renderer *renderer;
};

void setFont(Ram* ram);

void video_cleanup(SDLState &state);

void do_instruction();

void update_timers();




int main(int argc, char* args[]){

    if (argc != 2){
        std::cout << "Error: Not Enough Arguments. Usage ./chip8.exe [program file]" << std::endl;
        return 1;
    }

    std::string program_name = args[1];

    std::ifstream fin(program_name, std::ios::binary | std::ios::ate);
    if(!fin.is_open()){
        std::cout << "Error: Couldn't open file" << std::endl;
        return 1;
    }

    read_file(fin);

    SDLState state;

    if (!SDL_Init(SDL_INIT_VIDEO)){
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl; 
        return 1;
    }

    state.window = SDL_CreateWindow("CHIP-8 Emulator", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!state.window){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error creating window", state.window);
        video_cleanup(state);
        return 1;
    }

    state.renderer = SDL_CreateRenderer(state.window, nullptr);
    if (!state.renderer){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error creating renderer", state.window);
        video_cleanup(state);
        return 1;
    }
    
    Ram memory(4096);

    setFont(&memory);

    auto last_clock_time = std::chrono::high_resolution_clock::now();

    double instr_cycle_time = 0;
    double timer_cycle_time = 0;
    

    //Main loop
    bool running = true;
    while (running){

        auto current_clock_time = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> time_delta_duration = current_clock_time - last_clock_time;

        double time_delta = time_delta_duration.count();

        instr_cycle_time += time_delta;
        timer_cycle_time += time_delta;

        if (instr_cycle_time >= instruction_freq){
            do_instruction();
            instr_cycle_time = 0;
        }

        if (timer_cycle_time >= timer_freq){
            update_timers();
            timer_cycle_time = 0;
        }

        SDL_Event event{0};
        while (SDL_PollEvent(&event)){
            switch (event.type){
                case SDL_EVENT_QUIT: {
                    running = false;
                    break;
                }

            }
        }

        last_clock_time = current_clock_time;
    }
    
    video_cleanup(state);
    
    return 0;
}

void video_cleanup(SDLState &state){
    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.window);
    SDL_Quit();
}

void do_instruction(){
    return ; 
}

void update_timers(){
    return ; 
}

void setFont(Ram* ram){

    using font_character = std::vector<byte>;

    //Declaring pixel values for each hexdecmial value to be stored in memory

    std::vector<font_character> font;

    font_character zero = {0xF0, 0x90, 0x90, 0x90, 0xF0}; //0
    font.insert(font.end(), zero);

    font_character one = {0x20, 0x60, 0x20, 0x20, 0x70}; //1
    font.insert(font.end(), one);

    font_character two = {0xF0, 0x10, 0xF0, 0x80, 0xF0}; //2
    font.insert(font.end(), two);

    font_character three = {0xF0, 0x10, 0xF0, 0x10, 0xF0}; //3
    font.insert(font.end(), three);

    font_character four = {0x90, 0x90, 0xF0, 0x10, 0x10}; //4
    font.insert(font.end(), four);

    font_character five = {0xF0, 0x80, 0xF0, 0x10, 0xF0}; //5
    font.insert(font.end(), five);

    font_character six = {0xF0, 0x80, 0xF0, 0x90, 0xF0}; //6
    font.insert(font.end(), six);

    font_character seven = {0xF0, 0x10, 0x20, 0x40, 0x40}; //7
    font.insert(font.end(), seven);

    font_character eight = {0xF0, 0x90, 0xF0, 0x90, 0xF0}; //8
    font.insert(font.end(), zero);

    font_character nine = {0xF0, 0x90, 0xF0, 0x10, 0xF0}; //9
    font.insert(font.end(), nine);

    font_character A = {0xF0, 0x90, 0xF0, 0x90, 0x90}; //A
    font.insert(font.end(), A);

    font_character B = {0xE0, 0x90, 0xE0, 0x90, 0xE0}; //B
    font.insert(font.end(), B);

    font_character C = {0xF0, 0x80, 0x80, 0x80, 0xF0}; //C
    font.insert(font.end(), C);

    font_character D = {0xE0, 0x90, 0x90, 0x90, 0xE0}; //D
    font.insert(font.end(), D);

    font_character E = {0xF0, 0x80, 0xF0, 0x80, 0xF0}; //E
    font.insert(font.end(), E);

    font_character F = {0xF0, 0x80, 0xF0, 0x80, 0x80}; //F
    font.insert(font.end(), F);

    address memory_address = 0x050; //Sticking with standard font location

    for (int i = 0; i < font.size(); ++i){
        for (int j = 0; j < font[i].size(); ++j){
            ram->write_address(memory_address, font[i][j]);
            memory_address += 1;
        }
    }
}