#include "util.h"

bool IsKeyNumberOrLetter(SDL_Keycode key)
{
    return (key >= SDLK_0 && key <= SDLK_9) || (key >= SDLK_a && key <= SDLK_z);
}

