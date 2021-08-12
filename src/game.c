#include "game.h"

int CheckCommands(World* world, Commands* commands, CommandInput input);
int ApplyCommands(World* world, Commands* commands);

void CheckBindEntity(World* world, Commands* commands, SDL_Keycode key);
void CheckSelectEntity(World* world, Commands* commands, SDL_Keycode key);

CommandInput HandleInput()
{
    CommandInput input;

    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        input.key = event.key.keysym.sym;
        if (event.key.keysym.mod == KMOD_SHIFT)
        {
            input.type = COMMAND_INPUT_TYPE_BIND_ENTITY;
        }
        else
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_SPACE:
                    input.type = COMMAND_INPUT_TYPE_ACTION;
                    break;

                case SDLK_UP:
                case SDLK_DOWN:
                case SDLK_LEFT:
                case SDLK_RIGHT:
                    input.type = COMMAND_INPUT_TYPE_MOVE_ENTITY;
                    break;

                case SDLK_ESCAPE:
                    input.type = COMMAND_INPUT_TYPE_CLOSE_GAME;
                    break;

                default:
                    input.type = COMMAND_INPUT_TYPE_SELECT_ENTITY;
            }
        }
    }

    return input;
}

int HandleGameWorld(World* world, const CommandInput input)
{
    Commands commands;
    commands.index = 0;

    CheckCommands(world, &commands, input);

    ApplyCommands(world, &commands);

    return 0;
}

void ExportWorldToRendererInfo(World* world, RendererInfo* info)
{

}

int CheckCommands(World* world, Commands* commands, CommandInput input)
{
    switch (input.type)
    {
        case COMMAND_INPUT_TYPE_BIND_ENTITY:
            CheckBindEntity(world, commands, input.key);
            break;

        case COMMAND_INPUT_TYPE_SELECT_ENTITY:
            CheckSelectEntity(world, commands, input.key);
            break;
    }

    return 0;
}

int ApplyCommands(World* world, Commands* commands)
{
    for (size_t index = 0; index <= commands->index; ++index)
    {
        CommandArgs* args = &commands->array[index].args;
        switch (commands->array[index].type)
        {
            case COMMAND_TYPE_BIND_ENTITY:
//                ApplyBindEntity(world, );
                WorldBindKeyToEntity(world, args->entity, args->key);
                break;

            case COMMAND_TYPE_SELECT_ENTITY:
                WorldSetCursor(world, args->entity);
                break;
        }
    }

    return 0;
}

void CheckBindEntity(World* world, Commands* commands, SDL_Keycode key)
{
    if (WorldFindEntityByKey(world, key) == NULL)
    {
        SDL_Log("Game::CheckBindEntity: %d", key);
        WorldEntity* entity = WorldGetUnbindedEntity(world);
        if (entity != NULL)
        {
            commands->array[commands->index] = (Command) {
                .type = COMMAND_TYPE_BIND_ENTITY,
                .args = (CommandArgs) {
                    .entity = entity,
                    .key = key
                }
            };
            ++commands->index;
        }
    }
    SDL_Log("Game::CheckBindEntity: end");
}

void CheckSelectEntity(World* world, Commands* commands, SDL_Keycode key)
{
    WorldEntity* entity = WorldFindEntityByKey(world, key);
    if (entity != NULL)
    {
        SDL_Log("Game::CheckSelectEntity: %d", key);
        commands->array[commands->index] = (Command) {
            .type = COMMAND_TYPE_SELECT_ENTITY,
            .args = (CommandArgs) {.entity = entity},
        };
        ++commands->index;
    }
    SDL_Log("Game::CheckSelectEntity: end");
}

