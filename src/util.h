#ifndef UTIL_H
#define UTIL_H

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

#endif /* UTIL_H */
