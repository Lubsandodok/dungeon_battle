#include "world.h"

#include <stdlib.h>

void AddToPositions(World* world, WorldEntity* entity, Point position);

int WorldInit(World* world)
{
    // TODO use level instead of hardcode
    world->data.mines_count = 0;
    world->data.walls_count = 0;
    world->data.units_count = 0;
    world->data.shots_count = 0;

    world->data.index = 0;
    world->unbinded_entity = NULL;

    world->data.entities = (WorldEntity*) malloc(ENTITY_MAX_COUNT * sizeof(WorldEntity));

    // TODO 40 and 40
    world->positions.x_length = 40;
    world->positions.y_length = 40;
    world->positions = (WorldPosition**) malloc(positions.y_length * sizeof(WorldPosition*));
    for (size_t j = 0; j < 40; ++j)
    {
        world->positions[j] = (WorldPosition*) malloc(positions.x_length * sizeof(WorldPosition));
        for (size_t i = 0; i < 40; ++i)
        {
            world->positions[j][i].index = 0;
        }
    }

    // TODO
    return 0;
}

void WorldQuit(World* world)
{
    // free entites
    free(world->data.entities);

    // free positions
    for (size_t j = 0; j < positions.y_length; ++j)
    {
        free(world->positions[j]);
    }
    free(world->positions);

    // free groups
    WorldGroup* group;
    WorldGroup* tmp;
    HASH_ITER(hh, world->groups, group, tmp)
    {
        HASH_DEL(world->groups, group);
        free(group);
    }
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

WorldEntity* WorldGetUnbindedEntity(World* world)
{
    return world->unbinded_entity;
}

WorldCursor WorldGetCursor(World* world)
{
    return world->cursor;
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

int WorldCreateWall(World* world, Point position)
{
    world->data.entites[world->data.index] = (WorldEntity) {
        .type = WALL,
        .common = (Common) {
            .position = position
        }
    };
    AddToPositions(world, &world->data.entites[world->data.index], position);
    ++world->data.index;

    return 0;
}

int WorldCreateMine(World* world, Point position)
{
    world->data.entites[world->data.index] = (WorldEntity) {
        .type = MINE,
        .common = (Common) {
            .position = position
        },
        .data = (Mine) {
            // TODO default letter
            .is_base = is_base
        }
    };
    AddToPositions(world, &world->data.entites[world->data.index], position);
    ++world->data.index;

    return 0;
}

int WorldUpgradeMineToBase(World* world, WorldEntity* entity)
{
    // TODO check
    
}

int WorldCreateUnit(World* world, Point position)
{
    world->data.entites[world->data.index] = (WorldEntity) {
        .type = UNIT,
        .common = (Common) {
            .position = position
        },
        .data = (Unit) {
            // TODO default letter
            .direction = DIRECTION_UP
        }
    }
    AddToPositions(world, &world->data.entites[world->data.index], position);
    ++world->data.index;

    world->unbinded_entity = &world->data.entites[world->data.index];
    // TODO
    world->cursor.entity = NULL;
    
    return 0;
}

void AddToPositions(World* world, WorldEntity* entity, Point position)
{
    WorldPosition* field_position = &world->positions.data[position.x][position.y];
    field_position->entites[field_position->index] = entity;
    ++field_position->index;
}

