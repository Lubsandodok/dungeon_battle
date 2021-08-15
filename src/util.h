#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>

#include <SDL.h>

typedef enum
{
    DIRECTION_UP,
    DIRECTION_DOWN,
    DIRECTION_LEFT,
    DIRECTION_RIGHT,
} Direction;

typedef enum
{
    LAYOUT_VERTICAL,
    LAYOUT_HORIZONTAL,
} Layout;

typedef struct Point
{
    uint32_t x;
    uint32_t y;
} Point;

bool IsKeyNumberOrLetter(SDL_Keycode key);
Direction GetDirectionByKey(SDL_Keycode key);

#endif /* UTIL_H */
