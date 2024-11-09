#ifndef LIB_START_H_
#define LIB_START_H_

#include "board.h"
#include "draw.h"
#include "head.h"
#include "point.h"

#include <SDL3/SDL.h>

#include <stdbool.h>

#define BWW 400   // Board window width
#define BWH 400   // Board window height
#define SWW 800   // Screen window width
#define SWH 600   // Screen window height

#define BW 10     // Board width
#define BH 10     // Board height

#define PMI 0.15  // Player move increment
#define PTI 0.15  // Player turn increment

#define ND (double)(M_PI * 0.5)   // 90 degrees in radians
#define FOV 1.1519                // 66 degrees in radians

#define TS 64   // Tile size

#define SN 3    // Sprite number

#define STEPS 200

SDL_Window *sWindow, *bWindow;    // Screen window, board window
SDL_Renderer *sRender, *bRender;  // Screen renderer, board renderer

SDL_Texture *sTexture[TNUM];   // Screen texture
SDL_Texture *bTexture[TNUM];   // Board texture

Board board;

Player player;

int currentType = BRICK;

bool positionWindows(void)
{
    SDL_DisplayID *dID = SDL_GetDisplays(NULL);
    if (dID == NULL) return error("SDL_GetDisplays");

    SDL_Rect display;
    SDL_GetDisplayUsableBounds(dID[0], &display);

    if (!SDL_SetWindowPosition(sWindow, (display.w * 0.5) - ((SWW + BWW) * 0.5) + BWW + 10, SDL_WINDOWPOS_CENTERED))
        return error("SDL_SetWindowPosition screen");

    if (!SDL_SetWindowPosition(bWindow, (display.w * 0.5) - ((SWW + BWW) * 0.5) - 10, SDL_WINDOWPOS_CENTERED))
        return error("SDL_SetWindowPosition board");

    return true;
}

bool start(void)
{
    if (!SDL_Init(SDL_INIT_VIDEO)) return error("SDL_Init");

    if (!SDL_CreateWindowAndRenderer("screen", SWW, SWH, SDL_WINDOW_INPUT_FOCUS, &sWindow, &sRender))
        return error("SDL_CreateWindowAndRenderer screen");

    if (!SDL_CreateWindowAndRenderer("board", BWW, BWH, SDL_WINDOW_INPUT_FOCUS, &bWindow, &bRender))
        return error("SDL_CreateWindowAndRendere boardr");

    if (!positionWindows()) return false;

    if (!loadImages(bRender, sRender, bTexture, sTexture)) return false;

    if (!createBoard(&board, BW, BH, BWW / BW)) return false;

    setBoard(&board);

    placePlayer(&player, BW, BH);

    return true;
}

void stop(void)
{
    unloadImages(sTexture, bTexture);
    
    SDL_DestroyRenderer(sRender);
    SDL_DestroyRenderer(bRender);
    SDL_DestroyWindow(sWindow);
    SDL_DestroyWindow(bWindow);

    SDL_Quit();
}

#endif // LIB_START_H_
