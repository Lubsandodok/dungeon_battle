#include "renderer.h"
#include <stdlib.h>

typedef struct DrawContext
{
    uint32_t* pixels;
    int32_t pitch;
    uint32_t color;
    Point point;
    Point offset;
    const SDL_PixelFormat* format;
    const RendererEntity* entity;
    uint32_t sidePosition;
    uint32_t sideRect;
    uint32_t border;
} DrawContext;

uint32_t GetColorByType(const DrawContext* context);
void DrawDefaultEntity(const DrawContext* context);
void DrawUnitEntity(const DrawContext* context);
void DrawShotEntity(const DrawContext* context);
void DrawRect(const DrawContext* context);

int RendererContextInit(RendererContext* context, uint32_t width, uint32_t height)
{
    if (SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_RESIZABLE,
                &context->window, &context->renderer))
    {
        SDL_Log("RendererContextInit::SDL_CreateWindowAndRenderer: %s", SDL_GetError());
        return 1;
    }

    context->texture = SDL_CreateTexture(context->renderer, SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_STREAMING, width, height);
    if (context->texture == NULL)
    {
        SDL_Log("RendererContextInit::SDL_CreateTexture: %s", SDL_GetError());
        return 1;
    }

    context->format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
    if (context->format == NULL)
    {
        SDL_Log("RendererContextInit::SDL_CreateTexture: %s", SDL_GetError());
        return 1;
    }

    return 0;
}

void RendererContextQuit(RendererContext* context)
{
    SDL_DestroyTexture(context->texture);
    SDL_DestroyRenderer(context->renderer);
    SDL_DestroyWindow(context->window);
}

int RendererInfoInit(RendererInfo* info, uint32_t x_length,
        uint32_t y_length, uint32_t side, uint32_t border)
{
    info->x_length = x_length;
    info->y_length = y_length;
    info->side = side;
    info->border = border;

    info->entites = (RendererEntity**) malloc(y_length * sizeof(RendererEntity*));
    for (size_t j = 0; j < y_length; ++j)
    {
        info->entites[j] = (RendererEntity*) malloc(x_length * sizeof(RendererEntity));
    }

    // TODO
    return 0;
}

void RendererInfoQuit(RendererInfo* info)
{
    for (size_t j = 0; j < info->y_length; ++j)
    {
        free(info->entites[j]);
    }
    free(info->entites);
}

int Render(RendererContext* context, const RendererInfo* info)
{
    uint32_t* pixels = NULL;
    int32_t pitch;
    if (SDL_LockTexture(context->texture, NULL, (void**)&pixels, &pitch))
    {
        SDL_Log("Render::SDL_LockTexture: %s", SDL_GetError());
        return 1;
    }

    for (size_t i = 0; i < info->x_length; ++i)
    {
        for (size_t j = 0; j < info->y_length; ++j)
        {
            const RendererEntity* entity = &info->entites[i][j];
            DrawContext draw_context = {
                .pixels = pixels,
                .pitch = pitch,
                .point = {
                    .x = i,
                    .y = j
                },
                .offset = {
                    .x = 0,
                    .y = 0
                },
                .format = context->format,
                .entity = entity,
                .sidePosition = info->side,
                .sideRect = info->side,
                .border = info->border
            };

            switch (entity->type)
            {
                case RENDERER_ENTITY_TYPE_WALL:
                    draw_context.color = SDL_MapRGBA(context->format, 0, 0, 255, 255);
                    DrawDefaultEntity(&draw_context);
                    break;

                case RENDERER_ENTITY_TYPE_FLOOR:
                    draw_context.color = SDL_MapRGBA(context->format, 255, 255, 255, 255);
                    DrawDefaultEntity(&draw_context);
                    break;

                case RENDERER_ENTITY_TYPE_MINE:
                    draw_context.color = SDL_MapRGBA(context->format, 255, 255, 0, 255);
                    DrawDefaultEntity(&draw_context);
                    break;

                case RENDERER_ENTITY_TYPE_BASE:
                    draw_context.color = SDL_MapRGBA(context->format, 0, 0, 0, 255);
                    DrawDefaultEntity(&draw_context);
                    break;

                case RENDERER_ENTITY_TYPE_UNIT:
                    draw_context.color = SDL_MapRGBA(context->format, 0, 255, 0, 255);
                    DrawUnitEntity(&draw_context);
                    break;

                case RENDERER_ENTITY_TYPE_SHOT:
                    draw_context.color = SDL_MapRGBA(context->format, 0, 0, 0, 255);
                    DrawShotEntity(&draw_context);
                    break;
            }
        }
    }

    SDL_UnlockTexture(context->texture);

    if (SDL_RenderClear(context->renderer))
    {
        SDL_Log("Render::SDL_RenderClear: %s", SDL_GetError());
        return 1;
    }
    if (SDL_RenderCopy(context->renderer, context->texture, NULL, NULL))
    {
        SDL_Log("Render::SDL_RenderCopy: %s", SDL_GetError());
        return 1;
    }
    SDL_RenderPresent(context->renderer);

    return 0;
}

void DrawDefaultEntity(const DrawContext* context)
{
    DrawRect(context);
}

void DrawUnitEntity(const DrawContext* context)
{
    DrawRect(context);

    uint32_t side = context->sidePosition;
    DrawContext head = *context;
    head.color = SDL_MapRGBA(context->format, 0, 0, 0, 255);
    head.sideRect = side / 3;
    head.border = 0;
    switch(head.entity->data.unit.direction)
    {
        case DIRECTION_UP:
            head.offset = (Point) {
                .x = side / 3,
                .y = 0
            };
            break;
        
        case DIRECTION_DOWN:
            head.offset = (Point) {
                .x = side / 3,
                .y = 2 * side / 3
            };
            break;

        case DIRECTION_LEFT:
            head.offset = (Point) {
                .x = 0,
                .y = side / 3
            };
            break;

        case DIRECTION_RIGHT:
            head.offset = (Point) {
                .x = 2 * side / 3,
                .y = side / 3
            };
            break;
    }
    DrawRect(&head);
}

void DrawShotEntity(const DrawContext* context)
{}

void DrawRect(const DrawContext* context)
{
    const Point* point = &context->point;
    const Point* offset = &context->offset;
    uint32_t sidePos = context->sidePosition;
    uint32_t sideRect = context->sideRect;
    uint32_t border = context->border;
    for (size_t y = point->y * sidePos + border + offset->y;
            y < point->y * sidePos + sideRect - border + offset->y; ++y)
    {
        Uint32* p = (Uint32*) ((Uint8*) context->pixels + context->pitch * y);
        p += point->x * sidePos + border + offset->x;
        for (size_t x = point->x * sidePos + border + offset->x;
                x < point->x * sidePos + sideRect - border + offset->x;
                ++x)
        {
            *p = context->color;
            p++;
        }
    }
}

