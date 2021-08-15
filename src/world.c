#include "world.h"

#include <stdlib.h>

void AddToPositions(World* world, WorldEntity* entity, Point position);
void RemoveFromPosition(World* world, WorldEntity* entity, Point position);

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
    world->positions.data = (WorldPosition**) malloc(world->positions.y_length * sizeof(WorldPosition*));
    for (size_t j = 0; j < 40; ++j)
    {
        world->positions.data[j] = (WorldPosition*) malloc(world->positions.x_length * sizeof(WorldPosition));
        for (size_t i = 0; i < 40; ++i)
        {
            world->positions.data[j][i].index = 0;
        }
    }

    // TODO
    return 0;
}

void WorldQuit(World* world)
{
    // free entities
    free(world->data.entities);

    // free positions
    for (size_t j = 0; j < world->positions.y_length; ++j)
    {
        free(world->positions.data[j]);
    }
    free(world->positions.data);

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

WorldEntity* WorldGetEntityWithHighestPriority(World* world, Point position)
{
    WorldPosition* field_position = &world->positions.data[position.x][position.y];

    WorldEntityType priority = MINE;
    WorldEntity* entity = NULL;
    for (size_t i = 0; i < field_position->index; ++i)
    {
        if (priority <= field_position->entities[i]->type)
        {
            priority = field_position->entities[i]->type;
            entity = field_position->entities[i];
        }
    }

    return entity;
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

WorldEntity* WorldCreateWall(World* world, Point position)
{
    world->data.entities[world->data.index] = (WorldEntity) {
        .type = WALL,
        .common = (Common) {
            .position = position
        }
    };
    WorldEntity* created = &world->data.entities[world->data.index];
    ++world->data.index;

    AddToPositions(world, created, position);

    return created;
}

WorldEntity* WorldCreateMine(World* world, Point position)
{
    world->data.entities[world->data.index] = (WorldEntity) {
        .type = MINE,
        .common = (Common) {
            .position = position
        },
        .data.mine = (Mine) {
            // TODO default letter
            .letter = ' ',
            .is_base = false
        }
    };
    WorldEntity* created = &world->data.entities[world->data.index];
    ++world->data.index;

    AddToPositions(world, created, position);

    return created;
}

WorldEntity* WorldCreateUnit(World* world, Point position)
{
    world->data.entities[world->data.index] = (WorldEntity) {
        .type = UNIT,
        .common = (Common) {
            .position = position
        },
        .data.unit = (Unit) {
            // TODO default letter
            .letter = ' ',
            .direction = DIRECTION_UP
        }
    };
    WorldEntity* created = &world->data.entities[world->data.index];
    ++world->data.index;

    AddToPositions(world, created, position);

    world->unbinded_entity = created;
    // TODO
    world->cursor.entity = NULL;
    
    return created;
}

int WorldUpgradeMineToBase(World* world, WorldEntity* entity)
{
    // TODO check
    SDL_Log("Game::WorldUpgradeMineToBase");

    entity->data.mine.is_base = true;

    world->unbinded_entity = entity;
    // TODO
    world->cursor.entity = NULL;

    return 0;
}

int WorldMoveEntity(World* world, WorldEntity* entity, Direction direction)
{
    Point old = entity->common.position;
    Point new = old;
    switch (direction)
    {
        case DIRECTION_UP:
            ++new.y;
            break;

        case DIRECTION_DOWN:
            --new.y;
            break;

        case DIRECTION_LEFT:
            --new.x;
            break;

        case DIRECTION_RIGHT:
            ++new.x;
            break;
    }
    entity->common.position = new;


}

void AddToPositions(World* world, WorldEntity* entity, Point position)
{
    WorldPosition* field_position = &world->positions.data[position.x][position.y];
    field_position->entities[field_position->index] = entity;
    ++field_position->index;
}

void RemoveFromPosition(World* world, WorldEntity* entity, Point position)
{
    WorldPosition* field_position = &world->positions.data[position.x][position.y];
    for (size_t i = 0; i < field_position->index; ++i)
    {
        if (entity == field_position->entities[i])
        {

        }
    }
}

