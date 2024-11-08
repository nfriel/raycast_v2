#ifndef LIB_LOOP_H_
#define LIB_LOOP_H_

#include "start.h"
#include "board.h"
#include "point.h"
#include "draw.h"
#include "castB.h"

#include <SDL3/SDL.h>

#include <stdbool.h>

#define FT 1000 / 60

uint8_t currentType = 1;

void loop(void);
bool handleKeyInput(void);
bool handleMouseInput(void);
void draw(void);

void loop(void)
{
    bool running = true;
    SDL_Event event;
    int time, wait;

    while (running)
    {
        time = SDL_GetTicks();
        updatePlayer();

        handleKeyInput();
        handleMouseInput();

        draw();

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_EVENT_QUIT:
                running = false;
                break;

                default:
                break;
            }
        }
        wait = FT - (SDL_GetTicks() - time);
        if (wait < 0) wait = 0;
        SDL_Delay(wait);
    }
}

bool handleKeyInput(void)
{
    bool xHitPos, xHitNeg, yHitPos, yHitNeg;

    if (board.array[(int)SDL_floor(player.pos.y)][(int)SDL_floor(player.pos.x + (player.dir.x * PMI * 2))]) xHitPos = true;
    else xHitPos = false;
    if (board.array[(int)SDL_floor(player.pos.y)][(int)SDL_floor(player.pos.x - (player.dir.x * PMI * 2))]) xHitNeg = true;
    else xHitNeg = false;

    if (board.array[(int)SDL_floor(player.pos.y + (player.dir.y * PMI * 2))][(int)SDL_floor(player.pos.x)]) yHitPos = true;
    else yHitPos = false;
    if (board.array[(int)SDL_floor(player.pos.y - (player.dir.y * PMI * 2))][(int)SDL_floor(player.pos.x)]) yHitNeg = true;
    else yHitNeg = false;

    const bool *keys = SDL_GetKeyboardState(NULL);
    if (keys == NULL)
    {
        error("SDL_GetKeyboradState");
        return false;
    }

    if (keys[SDL_SCANCODE_UP])
    {
        if (!xHitPos) player.pos.x += player.dir.x * PMI;
        if (!yHitPos) player.pos.y += player.dir.y * PMI;
    }
    
    if (keys[SDL_SCANCODE_DOWN])
    {
        if (!xHitNeg) player.pos.x -= player.dir.x * PMI;
        if (!xHitPos) player.pos.y -= player.dir.y * PMI;
    }

    if (keys[SDL_SCANCODE_LEFT])
    {
        player.angle -= PTI;
    }

    if (keys[SDL_SCANCODE_RIGHT])
    {
        player.angle += PTI;
    }

    if (keys[SDL_SCANCODE_1])
    {
        currentType = 1;
    }

    if (keys[SDL_SCANCODE_2])
    {
        currentType = 2;
    }


       return true;
}

bool handleMouseInput(void)
{
    float x, y;
    SDL_MouseButtonFlags flags = SDL_GetMouseState(&x, &y);

    int bX = x / player.scale;
    int bY = y / player.scale;

    if (flags & SDL_BUTTON_LMASK) updateBoard(bX, bY, currentType);
    if (flags & SDL_BUTTON_RMASK) updateBoard(bX, bY, false);

    return true;
}

void draw(void)
{
    renderClear(boardRenderer, black);
    renderClear(screenRenderer, ceiling);

    drawBoard(boardRenderer);
    drawPlayer(boardRenderer);
    raycast(200);

    SDL_RenderPresent(boardRenderer);
    SDL_RenderPresent(screenRenderer);
}

#endif // LIB_LOOP_H_
