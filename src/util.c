#include "util.h"

bool IsKeyNumberOrLetter(SDL_Keycode key)
{
    return (key >= SDLK_0 && key <= SDLK_9) || (key >= SDLK_a && key <= SDLK_z);
}

Direction GetDirectionByKey(SDL_Keycode key)
{
    Direction direction;
    switch (key)
    {
        case SDLK_UP:
            direction = DIRECTION_UP;
            break;

        case SDLK_DOWN:
            direction = DIRECTION_DOWN;
            break;

        case SDLK_LEFT:
            direction = DIRECTION_LEFT;
            break;

        case SDLK_RIGHT:
            direction = DIRECTION_RIGHT;
            break;
    }

    return direction;
}

