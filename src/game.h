#ifndef GAME_H
#define GAME_H

#include "world.h"
#include "renderer.h"

#include <SDL.h>

typedef enum
{
    COMMAND_TYPE_BIND_ENTITY,
    COMMAND_TYPE_SELECT_ENTITY
} CommandType;

typedef struct CommandInput
{
    CommandType type;
    SDL_Keysym key;
} CommandInput;

typedef struct Command
{
    CommandType type;
// TODO
//    CommandArgs args;
} Command;

typedef struct Commands
{
    Command* list;
    uint32_t length;
} Commands;

CommandInput HandleInput();
int HandleGameWorld(World* world, const CommandInput input);
void ExportWorldToRendererInfo(World* world, RendererInfo* info);

#endif /* GAME_H */
