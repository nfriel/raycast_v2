#ifndef LIB_START_H_
#define LIB_START_H_

#include "draw.h"
#include "board.h"
#include "point.h"

#include <SDL3/SDL.h>

#include <stdbool.h>
#include <stdlib.h>

#define SWW 800     // Screen window width
#define SWH 600     // Screen window height
#define BWW 400     // Board window width
#define BWH 400     // Board window height

#define BW 20       // Board width
#define BH 20       // Board height

#define PMI 0.1     // Player move increment   
#define PTI 0.05    // Player turn increment

#define TS 64       // Tile size

SDL_Window *boardWindow, *screenWindow;
SDL_Renderer *boardRenderer, *screenRenderer;

bool start(void);
void stop(void);
bool positionWindows(void);
void error(const char *name);

bool start(void)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        error("SDL_Init");
        return false;
    }

    if (!SDL_CreateWindowAndRenderer("board", BWW, BWH, SDL_WINDOW_INPUT_FOCUS, &boardWindow, &boardRenderer))
    {
        error("SDL_CreateWindowAndRenderer");
        return false;
    }

    if (!SDL_CreateWindowAndRenderer("screen", SWW, SWH, SDL_WINDOW_INPUT_FOCUS, &screenWindow, &screenRenderer))
    {
        error("SDL_CreateWindowAndRenderer");
        return false;
    }

    if (!positionWindows()) return false;
    if (!loadTextures(boardRenderer, screenRenderer)) return false;
    if (!createBoard()) return false;

    setBoard();
    setPlayer(BW, BH, BWW, BWH);

    return true;
}

void stop(void)
{
    destroyBoard();

    destroyTextures(); 

    SDL_DestroyRenderer(boardRenderer);
    SDL_DestroyRenderer(screenRenderer);

    SDL_DestroyWindow(boardWindow);
    SDL_DestroyWindow(screenWindow);

}

bool positionWindows(void)
{
    SDL_DisplayID *dID = SDL_GetDisplays(NULL);
    if (dID == NULL)
    {
        error("SDL_GetDisplays");
        return false;
    }

    SDL_Rect display;
    SDL_GetDisplayUsableBounds(dID[0], &display);

    if (!SDL_SetWindowPosition(screenWindow, (display.w * 0.5) - ((SWW + BWW) * 0.5) + BWW + 10, SDL_WINDOWPOS_CENTERED))
    {
        error("SDL_SetWindowPosition");
        return false;
    }

    if (!SDL_SetWindowPosition(boardWindow, (display.w * 0.5) - ((SWW + BWW) * 0.5) - 10, SDL_WINDOWPOS_CENTERED))
    {
        error("SDL_SetWindowPosition");
        return false;
    }

    return true;
}

void error(const char *name)
{
    printf("%s error: %s\n", name, SDL_GetError());
}

#endif // LIB_START_H_
