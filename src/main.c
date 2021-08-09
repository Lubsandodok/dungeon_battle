#include <SDL.h>

#include "renderer.h"
#include "level.h"

#define WIDTH 1000
#define HEIGHT 1000
#define LENGTH 40

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_Log("main::SDL_Init %s", SDL_GetError());
        return 1;
    }

    RendererContext context;   
    if (RendererContextInit(&context, WIDTH, HEIGHT))
    {
        SDL_Log("main::RendererContextInit");
        return 1;
    }

    RendererInfo info;
    if (RendererInfoInit(&info, LENGTH, LENGTH, WIDTH / LENGTH, 1))
    {
        SDL_Log("main::RendererInfoInfo");
        return 1;
    }

    ExportLevelToRendererInfo(&info);

    Render(&context, &info);

    SDL_Event e;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
            if (e.type == SDL_KEYDOWN) {
                quit = 1;
            }
        }
        SDL_Delay(1000);
    }

    RendererInfoQuit(&info);
    RendererContextQuit(&context);
    SDL_Quit();

    return 0;
}
