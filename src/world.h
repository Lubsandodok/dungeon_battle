#ifndef WORLD_H
#define WORLD_H

#include <stdbool.h>
#include <uthash.h>
#include <SDL.h>

#include "util.h"

#define ENTITY_MAX_COUNT 50 * 50

// Data
typedef enum
{
    MINE,
    SHOT,
    WALL,
    UNIT,
} WorldEntityType;

typedef struct Common
{
    Point position;
} Common;

typedef struct Mine
{
    char letter;
    bool is_base;
} Mine;

typedef struct Wall
{} Wall;

typedef struct Unit
{
    char letter;
    Direction direction;
} Unit;

typedef struct Shot
{
    Layout layout;
} Shot;

typedef union
{
    Mine mine;
    Wall wall;
    Unit unit;
    Shot shot;
} WorldEntityData;

typedef struct WorldEntity
{
    WorldEntityType type;
    Common common;
    WorldEntityData data;
} WorldEntity;

typedef struct WorldData
{
    WorldEntity* entities;
    size_t index;

    uint32_t mines_count;
    uint32_t walls_count;
    uint32_t units_count;
    uint32_t shots_count;
} WorldData;


// Meta
typedef struct WorldPosition
{
    // HERE
    WorldEntity* entities[3];
    size_t index;
} WorldPosition;

typedef struct WorldPositions
{
    WorldPosition** data;
    size_t x_length;
    size_t y_length;
} WorldPositions;

typedef struct WorldGroup
{
    SDL_Keycode key;
    WorldEntity* entity;
    UT_hash_handle hh;
} WorldGroup;

typedef struct WorldCursor
{
    Point position;
    WorldEntity* entity;
} WorldCursor;


typedef struct World
{
    WorldData data;

    WorldPositions positions;
    WorldGroup* groups;
    WorldCursor cursor;
    WorldEntity* unbinded_entity;
} World;

// TODO Level info
int WorldInit(World* world);
void WorldQuit(World* world);

const WorldEntity* WorldFindEntityByKey(World* world, SDL_Keycode key);
WorldEntity* WorldGetUnbindedEntity(World* world);
WorldCursor WorldGetCursor(World* world);
WorldEntity* WorldGetEntityWithHighestPriority(World* world, Point position);

int WorldBindKeyToEntity(World* world, WorldEntity* entity, SDL_Keycode sdl_key);
int WorldSetCursor(World* world, WorldEntity* entity);
WorldEntity* WorldCreateWall(World* world, Point position);
WorldEntity* WorldCreateMine(World* world, Point position);
WorldEntity* WorldCreateUnit(World* world, Point position);
int WorldUpgradeMineToBase(World* world, WorldEntity* entity);
int WorldMoveEntity(World* world, WorldEntity* entity, Direction direction);

#endif /* WORLD_H */
