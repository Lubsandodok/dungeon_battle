#include "level.h"

static const int32_t width = 40;
static const int32_t height = 40;

static const char level[][40] = {
"========================================",
"=                                      =",
"= ================== ================= =",
"= ================== ================= =",
"= ================== ================= =",
"= ================== ================= =",
"= ================== ================= =",
"= ================== ================= =",
"= ================== ================= =",
"= ================== ================= =",
"=                    ================= =",
"= ================== ================= =",
"= ================== ================= =",
"= ================== ================= =",
"= ================== ================= =",
"= ================== ================= =",
"= ================== ================= =",
"= ================== ================= =",
"=                                      =",
"= ================== ================= =",
"= ================== ================= =",
"= ================== ================= =",
"= ================== ================= =",
"= ================== ================= =",
"= ================== ================= =",
"= ================== ================= =",
"= ================== ================= =",
"=M                                     =",
"= ================== ================= =",
"=                                      =",
"= ================== ================= =",
"=                                      =",
"= ================== ================= =",
"=                                      =",
"= ================== ================= =",
"=                                      =",
"= ================== ================= =",
"= ================== ================= =",
"=                                     B=",
"========================================"
};

void ExportLevelToRendererInfo(RendererInfo* info)
{
    for (size_t i = 0; i < width; ++i)
    {
        for (size_t j = 0; j < height; ++j)
        {
            RendererEntity* entity = &info->entites[i][j];
            switch (level[i][j])
            {
                case ' ':
                    entity->type = RENDERER_ENTITY_TYPE_FLOOR;
                    break;

                case '=':
                    entity->type = RENDERER_ENTITY_TYPE_WALL;
                    break;

                case 'M':
                    entity->type = RENDERER_ENTITY_TYPE_MINE;
                    break;

                case 'B':
                    entity->type = RENDERER_ENTITY_TYPE_BASE;
                    break;

                case 'U':
                    entity->type = RENDERER_ENTITY_TYPE_UNIT;
                    entity->data.unit = (RendererEntityUnit) {.direction = DIRECTION_UP};
                    break;
            }
        }
    }
}

void ExportLevelToWorld(World* world)
{
    for (size_t i = 0; i < width; ++i)
    {
        for (size_t j = 0; j < height; ++j)
        {
            Point position = {.x = i, .y = j};
            switch (level[i][j])
            {
                case '=':
                    WorldCreateWall(world, position);
                    break;

                case 'M':
                    WorldCreateMine(world, position);
                    break;

                case 'B':
                    WorldCreateMine(world, position);
                    // TODO use functions
                    // Upgrade Mine
                    
                    break;
            }
        }
    }
}

