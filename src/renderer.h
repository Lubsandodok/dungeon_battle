#ifndef RENDERER_H
#define RENDERER_H

#include <stdint.h>
#include <SDL.h>

#include "util.h"

typedef struct RendererContext
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_PixelFormat* format;
} RendererContext;

typedef enum RendererEntityType
{
    RENDERER_ENTITY_TYPE_WALL,
    RENDERER_ENTITY_TYPE_FLOOR,
    RENDERER_ENTITY_TYPE_MINE,
    RENDERER_ENTITY_TYPE_BASE,
    RENDERER_ENTITY_TYPE_UNIT,
    RENDERER_ENTITY_TYPE_SHOT,
} RendererEntityType;

typedef struct RendererEntityUnit
{
    Direction direction;
} RendererEntityUnit;

typedef struct RendererEntityShot
{
    Layout layout;
} RendererEntityShot;

typedef union
{
    RendererEntityUnit unit;
    RendererEntityShot shot;
} RendererEntityData;

typedef struct RendererEntity
{
    RendererEntityData data;
    RendererEntityType type;
} RendererEntity;

typedef struct RendererInfo
{
    RendererEntity** entites;
    uint32_t x_length;
    uint32_t y_length;
    uint32_t side;
    uint32_t border;
} RendererInfo;

int RendererContextInit(RendererContext* context, uint32_t width, uint32_t height);
void RendererContextQuit(RendererContext* context);

int RendererInfoInit(RendererInfo* info, uint32_t x_length,
        uint32_t y_length, uint32_t side, uint32_t border);
void RendererInfoQuit(RendererInfo* info);

int Render(RendererContext* context, const RendererInfo* info);

#endif /* RENDERER_H */
