#include "screen.h"
#include <SDL2/SDL.h>
#include <stdio.h>

void sdl_init_screen(SDL_SCREEN *sdl){
    sdl->width = WINDOW_WIDTH;
    sdl->height = WINDOW_HEIGHT;
    

    sdl->window = SDL_CreateWindow(WINDOW_TITLE, 
                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                sdl->width * SCALE, sdl->height * SCALE, SDL_WINDOW_SHOWN);
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
    (void) sdl;
}
void sdl_draw_screen(SDL_SCREEN *sdl){
    SDL_SetRenderDrawColor(sdl->renderer, 0, 0, 0, 255);
    SDL_RenderClear(sdl->renderer);

    SDL_SetRenderDrawColor(sdl->renderer, 255, 255, 255, 255);
    SDL_RenderDrawPoint(sdl->renderer, sdl->width/2, sdl->height/2);

    SDL_RenderPresent(sdl->renderer);
}