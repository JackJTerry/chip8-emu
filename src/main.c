#include "screen.h"
#include <stdbool.h>

int main(void)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0){
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }

    SDL_SCREEN sdl = {0};
    sdl_init_screen(&sdl);

    while(true) {
        SDL_Event event;
        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT) {
            break;
        }

    }

    SDL_DestroyWindow(sdl.window);
    SDL_DestroyRenderer(sdl.renderer);
    SDL_Quit();

    return 0;
}
