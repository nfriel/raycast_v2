#ifndef LIB_LOOP_H_
#define LIB_LOOP_H_

#include "cast.h"
#include "start.h"
#include "draw.h"

#include <SDL3/SDL.h>

void handleKeyInput(void)
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

    if (keys[SDL_SCANCODE_UP])
    {
        if (!xHitPos) player.pos.x += player.dir.x * PMI;
        if (!yHitPos) player.pos.y += player.dir.y * PMI;
    }

    if (keys[SDL_SCANCODE_DOWN])
    {
        if (!xHitNeg) player.pos.x -= player.dir.x * PMI;
        if (!yHitNeg) player.pos.y -= player.dir.y * PMI;
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
        currentType = BRICK;
    }

    if (keys[SDL_SCANCODE_2])
    {
        currentType = WINDOW;
    }

    if (keys[SDL_SCANCODE_3])
    {
        currentType = GUY;
    }

    if (player.angle > M_PI * 2) player.angle = 0;
    if (player.angle < 0) player.angle = M_PI * 2;
}

bool handleMouseInput(void)
{
    float x, y;
    SDL_MouseButtonFlags flags = SDL_GetMouseState(&x, &y);

    int bX = x / board.scale;
    int bY = y / board.scale;

    if (flags & SDL_BUTTON_LMASK) updateBoard(&board, bX, bY, currentType);
    if (flags & SDL_BUTTON_RMASK) updateBoard(&board, bX, bY, FLOOR);

    return true;
}

void draw(void)
{
    setColor(bRender, black);
    setColor(sRender, black);
    SDL_RenderClear(bRender);
    SDL_RenderClear(sRender);

    drawBoard(bRender, bTexture, board);
    drawPlayer(bRender, bTexture, board, player);
    raycast(STEPS);
    sortSprites();
    drawSprite(STEPS);

    SDL_RenderPresent(bRender);
    SDL_RenderPresent(sRender);
}

void loop(void)
{
    bool running = true;
    SDL_Event event;

    while (running)
    {
        handleKeyInput();
        handleMouseInput();

        updatePlayer(&player);

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
    }
}

#endif // LIB_LOOP_H_
