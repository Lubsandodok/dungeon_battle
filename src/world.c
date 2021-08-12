#include "world.h"

#include <stdlib.h>

int WorldInit(World* world)
{
    // TODO use level instead of hardcode
    world->data.mines_count = 0;
    world->data.walls_count = 0;
    world->data.units_count = 0;
    world->data.shots_count = 0;

    world->unbinded_entity = NULL;

    world->data.entities = (WorldEntity*) malloc(50 * 50 * sizeof(WorldEntity));

    // TODO
    return 0;
}

void WorldQuit(World* world)
{
    // TODO
    free(world->data.entities);
}

const WorldEntity* WorldFindEntityByKey(World* world, SDL_Keycode key)
{
    WorldGroup* group = NULL;
    HASH_FIND_INT(world->groups, &key, group);
    if (group != NULL)
    {
        return group->entity;
    }
    return NULL;
}

// TODO
WorldCursor GetCursor(World* world)
{
    return world->cursor;
}

WorldEntity* WorldGetUnbindedEntity(World* world)
{
    return world->unbinded_entity;
}

int WorldBindKeyToEntity(World* world, WorldEntity* entity, SDL_Keycode sdl_key)
{
    SDL_Log("World::BindKeyToEntity: %d", sdl_key);

    WorldGroup* group = (WorldGroup*) malloc(sizeof(WorldGroup));
    group->entity = entity;
    group->key = sdl_key;

    HASH_ADD_INT(world->groups, key, group);

    // TODO add to positions

    // TODO set key to world data (2)

    return 0;
}

int WorldSetCursor(World* world, WorldEntity* entity)
{
    world->cursor.entity = entity;
    world->cursor.position = entity->common.position;
    return 0;
}

