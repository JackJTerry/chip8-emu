#include "screen.h"
#include <SDL2/SDL.h>
#include <stdio.h>

void sdl_init_screen(SDL_SCREEN *sdl){
    sdl->width = WINDOW_WIDTH;
    sdl->height = WINDOW_HEIGHT;
    

    sdl->window = SDL_CreateWindow(WINDOW_TITLE, 
                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                sdl->width * SCALE, sdl->height * SCALE, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
    if(sdl->window == NULL){
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
    }

    sdl->renderer = SDL_CreateRenderer(sdl->window, -1, 0);
    if(sdl->renderer == NULL){
        printf( "Renderer could not be created! SDL_Error: %s\n", SDL_GetError() );
    }

    sdl->texture = SDL_CreateTexture(sdl->renderer,
                                    SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
                                    sdl->width, sdl->height);
    if(sdl->texture == NULL){
        printf( "Texture could not be created! SDL_Error: %s\n", SDL_GetError() );
    }

    SDL_RenderSetScale(sdl->renderer, SCALE, SCALE);
}
void sdl_destroy_screen(SDL_SCREEN *sdl){
    SDL_DestroyWindow(sdl->window);
    SDL_DestroyRenderer(sdl->renderer);
    SDL_DestroyTexture(sdl->texture);
    SDL_Quit();
}
void sdl_draw_screen(SDL_SCREEN *sdl, CHIP8 *chip8){
    SDL_SetRenderDrawColor(sdl->renderer, 0, 0, 0, 255);
    SDL_RenderClear(sdl->renderer);

    SDL_SetRenderDrawColor(sdl->renderer, 255, 255, 255, 255);
    for (int y = 0; y < WINDOW_HEIGHT; y++){
        for (int x = 0; x < WINDOW_WIDTH; x++) {
            if (chip8->graphics[x + (y * 64)]) {
                SDL_Rect rect;

                rect.x = x;
                rect.y = y;
                rect.w = 1;
                rect.h = 1;

                SDL_RenderFillRect(sdl->renderer, &rect);
            }
        }
    }

    SDL_RenderPresent(sdl->renderer);
}