#ifndef LIB_HEAD_H_
#define LIB_HEAD_H_

#include <SDL3/SDL.h>

bool error(const char *name)
{
    printf("%s error %s\n", name, SDL_GetError());
    return false;
}

#endif // LIB_HEAD_H_
