#ifndef GAME_H
#define GAME_H

#include "world.h"
#include "renderer.h"

#include <SDL.h>

typedef enum
{
    COMMAND_INPUT_TYPE_BIND_ENTITY,
    COMMAND_INPUT_TYPE_SELECT_ENTITY,
    COMMAND_INPUT_TYPE_MOVE_ENTITY,
    COMMAND_INPUT_TYPE_CHANGE_DIRECTION,
    COMMAND_INPUT_TYPE_ACTION,
    COMMAND_INPUT_TYPE_CLOSE_GAME,
} CommandInputType;

typedef struct CommandInput
{
    CommandInputType type;
    SDL_Keycode key;
} CommandInput;

typedef enum
{
    COMMAND_TYPE_BIND_ENTITY,
    COMMAND_TYPE_SELECT_ENTITY,
    COMMAND_TYPE_CREATE_BASE,
    COMMAND_TYPE_CREATE_UNIT,
    COMMAND_TYPE_MOVE_ENTITY,
    COMMAND_TYPE_CHANGE_DIRECTION,
    COMMAND_TYPE_SHOOT,
    COMMAND_TYPE_DESTROY_BASE,

    COMMAND_TYPE_MISS_SHOT,
    COMMAND_TYPE_DIE_UNIT,
} CommandType;

typedef struct CommandArgs
{
    WorldEntity* entity;
    SDL_Keycode key;
} CommandArgs;

typedef struct Command
{
    CommandType type;
    CommandArgs args;
} Command;

typedef struct Commands
{
    Command array[10];
    size_t index;
} Commands;

CommandInput HandleInput();
int HandleGameWorld(World* world, const CommandInput input);
void ExportWorldToRendererInfo(World* world, RendererInfo* info);

#endif /* GAME_H */
