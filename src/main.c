#include "screen.h"
#include "chip8.h"
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <cimgui.h>
#include <cimgui_impl_sdl2.h>
#include <cimgui_impl_sdlrenderer2.h>

int main(int argc, char *argv[])
{
     if (argc <= 1) {
        puts("ERROR : Please supply arguement");
        return -1;
    } 
    char *file_name = argv[1];

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0){
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }

    SDL_SCREEN sdl = {0};
    CHIP8 chip8 = {0};
    sdl_init_screen(&sdl);
    chip8_init(&chip8);
    chip8_load_rom(&chip8, file_name);
    Uint32 frame_start, frame_time;

    while(true) {
        frame_start = SDL_GetTicks();
        
        SDL_Event e;
        SDL_PollEvent(&e);
        chip8_handle_keypad(&chip8, &e);


        chip8_emu_cycle(&chip8);
        if (chip8.draw_flag){
            sdl_draw_screen(&sdl, &chip8);
        }

        if(e.type == SDL_QUIT) {
            break;
        }

        if(e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_ESCAPE) break;
        }
        frame_time = SDL_GetTicks() - frame_start;
        if (FRAME_DELAY > frame_time) {
            SDL_Delay(FRAME_DELAY - frame_time);
        }
    }
    puts("exiting");
    sdl_destroy_screen(&sdl);
    SDL_Quit();
    return 0;
}
