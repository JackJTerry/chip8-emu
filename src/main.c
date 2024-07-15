#include "screen.h"
#include "chip8.h"
#include <stdbool.h>
#include <unistd.h>

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

    while(true) {
        SDL_Event event;
        SDL_PollEvent(&event);

        chip8_execute_instruction(&chip8);
        if (chip8.draw_flag) sdl_draw_screen(&sdl, &chip8);

        if(event.type == SDL_QUIT) {
            break;
        }
        sleep(0.002);
    }

    sdl_destroy_screen(&sdl);
    return 0;
}
