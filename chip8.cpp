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

byte delay_timer = 0x0; //Decrements at 60 Hz

byte sound_timer = 0x0; //Beeps at 0;

std::vector<std::vector<bool>> pixels; //Matrix of bools representing all pixels

struct SDLState {
    SDL_Window *window;
    SDL_Renderer *renderer;
};

void setFont(Ram* ram);

void video_cleanup(SDLState &state);

void do_instruction();

void update_timers();

void print_video_memory();

void draw(SDLState &state);

bool debug_mode = false;

int main(int argc, char* args[]){

    if ((argc < 2) || (argc > 3)){
        std::cout << "Error: Not Enough Arguments. Usage ./chip8.exe program_file {debug}" << std::endl;
        return 1;
    }

    if (argc == 3){
        debug_mode = true;
    }

    std::string program_name = args[1];

    std::ifstream fin(program_name, std::ios::binary | std::ios::ate);
    if(!fin.is_open()){
        std::cout << "Error: Couldn't open file" << std::endl;
        return 1;
    }

    Ram memory(4096);

    memory.zeroOut();

    SDLState state;

    read_file(fin, &memory);

    for (int i = 0; i < 32; ++i){
        std::vector<bool> row;
        row.clear();
        for (int j = 0; j < 64; ++j){
            row.push_back(false);
        }
        pixels.push_back(row);
    }

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
    
    setFont(&memory);

    auto last_clock_time = std::chrono::high_resolution_clock::now();

    double instr_cycle_time = 0;
    double timer_cycle_time = 0;
    
    std::map<SDL_Scancode, bool> keyState;

    draw(state);

    //Main loop
    bool running = true;
    while (running){
        SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 0);
        SDL_RenderClear(state.renderer);

        auto current_clock_time = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> time_delta_duration = current_clock_time - last_clock_time;

        double time_delta = time_delta_duration.count();

        instr_cycle_time += time_delta;
        timer_cycle_time += time_delta;

        if (instr_cycle_time >= instruction_freq && !debug_mode){
            decode_and_excute(&memory, pixels, keyState, delay_timer, sound_timer);
            instr_cycle_time = 0;

        }

        if (timer_cycle_time >= timer_freq && !debug_mode ){
            update_timers();
            timer_cycle_time = 0;
        }

        if (instr_cycle_time >= 0.25 && debug_mode && keyState[SDL_SCANCODE_PERIOD]){
            decode_and_excute(&memory, pixels, keyState, delay_timer, sound_timer);
            instr_cycle_time = 0;
        }

        if (instr_cycle_time >= 0.25 && debug_mode && keyState[SDL_SCANCODE_M]){
            print_video_memory();
            instr_cycle_time = 0;
        }

        if (instr_cycle_time >= 0.25 && debug_mode && keyState[SDL_SCANCODE_P]){
            print_data();
            instr_cycle_time = 0;
        }

        SDL_Event event{0};
        while (SDL_PollEvent(&event)){
            switch (event.type){
                case SDL_EVENT_QUIT: {
                    running = false;
                    break;
                }
                case SDL_EVENT_KEY_DOWN: {
                    keyState[event.key.scancode] = true;
                    break;
                }
                case SDL_EVENT_KEY_UP: {
                    keyState[event.key.scancode] = false;
                }
            }
        }

        draw(state);
        
        last_clock_time = current_clock_time;
        SDL_RenderPresent(state.renderer);
    }
    
    video_cleanup(state);
    
    return 0;
}

void video_cleanup(SDLState &state){
    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.window);
    SDL_Quit();
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

void print_video_memory(){
    for (int i = 0; i < 32; ++i){
        for (int j = 0; j < 64; ++j){
            std::cout << pixels[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void draw(SDLState &state){
    SDL_SetRenderDrawColor(state.renderer, 255, 255, 255, 255);

    for (int i = 0; i < 32; ++i){
        for (int j = 0; j < 64; ++j){
            //std::cout << pixels[i][j] << " ";
            if (pixels[i][j]){
                SDL_FRect pixel = {float(j * window_factor), float(i * window_factor), float(window_factor), float(window_factor)};
                SDL_RenderFillRect(state.renderer, &pixel);
            }
        }
        //std::cout << std::endl;
    }
    SDL_RenderPresent(state.renderer);
}