#include "world.h"

#include <stdlib.h>

int WorldInit(World* world)
{
    // TODO use level instead of hardcode
    world->data.mines_count = 0;
    world->data.walls_count = 0;
    world->data.units_count = 0;
    world->data.shots_count = 0;

    world->data.entities = (WorldEntity*) malloc(50 * 50 * sizeof(WorldEntity));

    // TODO
    return 0;
}

void WorldQuit(World* world)
{
    free(world->data.entities);
}

