#ifndef WORLD_H
#define WORLD_H

#include <stdbool.h>
#include <uthash.h>
#include <SDL.h>

#include "util.h"

// Data
typedef enum
{
    MINE,
    WALL,
    UNIT,
    SHOT,
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

    uint32_t mines_count;
    uint32_t walls_count;
    uint32_t units_count;
    uint32_t shots_count;
} WorldData;


// Meta
typedef struct WorldPositions
{
    Point position;
    WorldEntity* entity;
    UT_hash_handle hh;
} WorldPositions;

typedef struct WorldGroups
{
    SDL_Keysym key;
    WorldEntity* entity;
    UT_hash_handle hh;
} WorldGroups;

typedef struct WorldCursor
{
    Point position;
    WorldEntity* entity;
} WorldCursor;


typedef struct World
{
    WorldData data;

    WorldPositions positions;
    WorldGroups groups;
    WorldCursor cursor;
} World;

// TODO Level info
int WorldInit(World* world);
void WorldQuit(World* world);

#endif /* WORLD_H */
