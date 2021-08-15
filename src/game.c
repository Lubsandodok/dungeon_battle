#include "game.h"
#include "util.h"

int CheckCommands(World* world, Commands* commands, CommandInput input);
int ApplyCommands(World* world, Commands* commands);

void CheckBindEntity(World* world, Commands* commands, SDL_Keycode key);
void CheckSelectEntity(World* world, Commands* commands, SDL_Keycode key);
void CheckMoveEntity(World* world, Commands* commands, SDL_Keycode key);
void CheckAction(World* world, Commands* commands);

CommandInput HandleInput()
{
    CommandInput input;

    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        SDL_Keycode sym = event.key.keysym.sym;
        SDL_Keymod mod = event.key.keysym.mod;
        input.key = sym;
        if (mod & KMOD_SHIFT)
        {
            if (IsKeyNumberOrLetter(sym))
            {
                input.type = COMMAND_INPUT_TYPE_BIND_ENTITY;
            }
        }
        else
        {
            if (sym == SDLK_SPACE)
            {
                input.type = COMMAND_INPUT_TYPE_ACTION;
            }
            else if (sym == SDLK_UP || sym == SDLK_DOWN || sym == SDLK_LEFT || sym == SDLK_RIGHT)
            {
                input.type = COMMAND_INPUT_TYPE_MOVE_ENTITY;
            }
            else if (sym == SDLK_ESCAPE)
            {
                input.type = COMMAND_INPUT_TYPE_CLOSE_GAME;
            }
            else if (IsKeyNumberOrLetter(sym))
            {
                input.type = COMMAND_INPUT_TYPE_SELECT_ENTITY;
            }
        }
    }

    return input;
}

int HandleGameWorld(World* world, const CommandInput input)
{
    // TODO
    if (input.type == COMMAND_INPUT_TYPE_NONE)
    {
        return 0;
    }

    Commands commands;
    commands.index = 0;

    CheckCommands(world, &commands, input);
    SDL_Log("Game::HandleGameWorld-AfterCheck: %d %d", input.key, commands.index);

    ApplyCommands(world, &commands);
    SDL_Log("Game::HandleGameWorld-AfterApply: %d %d", input.key, commands.index);

    return 0;
}

void ExportWorldToRendererInfo(World* world, RendererInfo* info)
{
    for (size_t i = 0; i < world->positions.x_length; ++i)
    {
        for (size_t j = 0; j < world->positions.y_length; ++j)
        {
            WorldEntity* we = WorldGetEntityWithHighestPriority(world, (Point) {.x = i, .y = j});
            RendererEntity* re = &info->entites[i][j];

//            SDL_Log("Game::ExportWorldToRendererInfo %d %d", i, j);

            if (we == NULL)
            {
                re->type = RENDERER_ENTITY_TYPE_FLOOR;
            }
            else if (we->type == MINE && !we->data.mine.is_base)
            {
                re->type = RENDERER_ENTITY_TYPE_MINE;
            }
            else if (we->type == MINE && we->data.mine.is_base)
            {
                re->type = RENDERER_ENTITY_TYPE_BASE;
            }
            else if (we->type == SHOT)
            {
                // TODO
            }
            else if (we->type == WALL)
            {
                re->type = RENDERER_ENTITY_TYPE_WALL;
            }
            else if (we->type == UNIT)
            {
                re->type = RENDERER_ENTITY_TYPE_UNIT;
                re->data.unit = (RendererEntityUnit) {.direction = DIRECTION_UP};
            }
        }
    }
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

        case COMMAND_INPUT_TYPE_MOVE_ENTITY:
            CheckMoveEntity(world, commands, input.key);
            break;

        case COMMAND_INPUT_TYPE_ACTION:
            CheckAction(world, commands);
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

            case COMMAND_TYPE_MOVE_ENTITY:
                WorldMoveEntity(world, args->entity, args->direction);
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

void CheckMoveEntity(World* world, Commands* commands, SDL_Keycode key)
{
    WorldEntity* entity = WorldFindEntityByKey(world, key);
    if (entity != NULL)
    {
        // TODO check borders
        Direction direction = GetDirectionByKey(key);
        commands->array[commands->index] = (Command) {
            .type = COMMAND_TYPE_MOVE_ENTITY,
            .args = (CommandArgs) {.entity = entity, .direction = direction},
        };
        ++commands->index;
    }

    SDL_Log("Game::CheckMoveEntity: end");
}

void CheckAction(World* world, Commands* commands)
{
    WorldCursor cursor = WorldGetCursor(world);

    // TODO
    if (cursor.entity == NULL)
    {
        return;
    }

    switch (cursor.entity->type)
    {
        case MINE:
            WorldCreateUnit(world, cursor.position);
            break;
    }
}

