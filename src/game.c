#include "game.h"

CommandInput HandleInput()
{
    // TODO
    CommandInput input;

    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        input.key = event.key.keysym;
    }
    return input;
}

int HandleGameWorld(World* world, const CommandInput input)
{
    Commands commands;

//    CheckCommands(world, commands, input);

//    ApplyCommands(world, commands);

    return 0;
}

void ExportWorldToRendererInfo(World* world, RendererInfo* info)
{

}

