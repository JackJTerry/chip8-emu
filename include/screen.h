#pragma once
#include <SDL2/SDL.h>

#define WINDOW_TITLE "CHIP 8 EMU"
#define WINDOW_HEIGHT 32
#define WINDOW_WIDTH 64
#define SCALE 10

typedef struct SDL_SCREEN{
    int width;
    int height;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
}SDL_SCREEN;

void sdl_init_screen(SDL_SCREEN *sdl);
void sdl_destroy_screen(SDL_SCREEN *sdl);
void sdl_draw_screen(SDL_SCREEN *sdl);