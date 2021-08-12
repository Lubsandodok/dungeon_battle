#include <stdbool.h>
#include <SDL.h>

#include "renderer.h"
#include "level.h"
#include "game.h"
#include "world.h"

#define WIDTH 1000
#define HEIGHT 1000
#define LENGTH 40

#define TICKS_PER_UPDATE 17

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_Log("main::SDL_Init %s", SDL_GetError());
        return 1;
    }

    // Create world
    World world;
    if (WorldInit(&world))
    {
        SDL_Log("main::WorldInit");
        return 1;
    }

    // Prepare renderer
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

    // Create level
    ExportLevelToRendererInfo(&info);

    // Create game logic
    uint32_t previous = SDL_GetTicks();
    uint32_t lag = 0;
    bool quit = false;
    while (!quit)
    {
        uint32_t current = SDL_GetTicks();
        uint32_t elapsed = current - previous;
        previous = current;
        lag += elapsed;

        // Input
        CommandInput input = HandleInput();
        if (input.type == COMMAND_INPUT_TYPE_CLOSE_GAME)
        {
            quit = true;
        }

        while (lag >= TICKS_PER_UPDATE)
        {
            // TODO
            HandleGameWorld(&world, input);

            ExportWorldToRendererInfo(&world, &info);

            Render(&context, &info);

            lag -= TICKS_PER_UPDATE;
        }

        // TODO
        SDL_Delay(10);
    }

    RendererInfoQuit(&info);
    RendererContextQuit(&context);
    WorldQuit(&world);
    SDL_Quit();

    return 0;
}
