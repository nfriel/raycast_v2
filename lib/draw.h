#ifndef LIB_DRAW_H_
#define LIB_DRAW_H_

#include "board.h"
#include "point.h"

#include <SDL3/SDL.h>

#define TS 64

SDL_Color white = { 255, 255, 255, 255 };
SDL_Color black = { 0, 0, 0, 255 };
SDL_Color red = { 255, 0, 0, 255 };
SDL_Color green = { 0, 255, 0, 255 };
SDL_Color blue = { 0, 0, 255, 255 };

SDL_Color gray = { 0, 0, 0, 100 };
SDL_Color ceiling = { 32, 60, 86, 255 };

SDL_Texture *bBrick, *bBrickFloor, *bBrickWindow;
SDL_Texture *sBrick, *sBrickFloor, *sBrickWindow;

bool loadTextures(SDL_Renderer *rendererA, SDL_Renderer *rendererB);
void destroyTextures(void);
SDL_FRect getIndexRect(int index);
void setColor(SDL_Renderer *renderer, SDL_Color color);
void drawRect(SDL_Renderer *renderer, int x, int y, int w, int h);
void drawRectOffset(SDL_Renderer *renderer, int x, int y, int w, int h);
void drawBoard(SDL_Renderer *renderer);
void drawPlayer(SDL_Renderer *renderer);
void renderClear(SDL_Renderer *renderer, SDL_Color color);

bool loadTextures(SDL_Renderer *renderA, SDL_Renderer *renderB)
{
    SDL_Surface *brick = SDL_LoadBMP("assets/brick.bmp");
    SDL_Surface *brickFloor = SDL_LoadBMP("assets/brickFloor.bmp");
    SDL_Surface *brickWindow = SDL_LoadBMP("assets/brickWindow.bmp");

    if (brick == NULL || brickFloor == NULL || brickWindow == NULL)
    {
        printf("SDL_LoadBMP error %s\n", SDL_GetError());
        return false;
    }

    bBrick = SDL_CreateTextureFromSurface(renderA, brick);
    bBrickFloor = SDL_CreateTextureFromSurface(renderA, brickFloor);
    bBrickWindow = SDL_CreateTextureFromSurface(renderA, brickWindow);
    
    if (bBrick == NULL || bBrickFloor == NULL || bBrickWindow == NULL)
    {
        printf("SDL_CreateTextureFromSurface A error %s\n", SDL_GetError());
        return false;
    }

    sBrick = SDL_CreateTextureFromSurface(renderB, brick);
    sBrickFloor = SDL_CreateTextureFromSurface(renderB, brickFloor);
    sBrickWindow = SDL_CreateTextureFromSurface(renderB, brickWindow);

    if (sBrick == NULL || sBrickFloor == NULL || bBrickWindow == NULL)
    {
        printf("SDL_CreateTextureFromSurface B error %s\n", SDL_GetError());
        return false;
    }

    // Prevent blurry upscaling on screen
    SDL_SetTextureScaleMode(sBrick, SDL_SCALEMODE_NEAREST);
    SDL_SetTextureScaleMode(sBrickFloor, SDL_SCALEMODE_NEAREST);
    SDL_SetTextureScaleMode(sBrickWindow, SDL_SCALEMODE_NEAREST);

    SDL_SetRenderDrawBlendMode(renderA, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawBlendMode(renderB, SDL_BLENDMODE_BLEND);

    SDL_DestroySurface(brick);
    SDL_DestroySurface(brickFloor);
    SDL_DestroySurface(brickWindow);

    return true;
}

void destroyTextures(void)
{
    SDL_DestroyTexture(bBrick);
    SDL_DestroyTexture(bBrickFloor);
    SDL_DestroyTexture(sBrick);
    SDL_DestroyTexture(sBrickFloor);    
}

SDL_FRect getIndexRect(int index)
{
    return (SDL_FRect)
    {
        .x = index,
        .y = 0,
        .w = 1,
        .h = TS
    };
}

void setColor(SDL_Renderer *renderer, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void drawRect(SDL_Renderer *renderer, int x, int y, int w, int h)
{
    SDL_FRect rect =
    {
        .x = x,
        .y = y,
        .w = w,
        .h = h
    };

    SDL_RenderFillRect(renderer, &rect);
}

void drawRectOffset(SDL_Renderer *renderer, int x, int y, int w, int h)
{
    SDL_FRect rect =
    {
        .x = x - (w * 0.5),
        .y = y - (h * 0.5),
        .w = w,
        .h = h
    };

    SDL_RenderFillRect(renderer, &rect);
}

void drawBoard(SDL_Renderer *renderer)
{
    SDL_Texture *temp;
    for (int i = 0; i < BH; i++)
    {
        for (int j = 0; j < BW; j++)
        {
            SDL_FRect dst =
            {
                .x = j * player.scale,
                .y = i * player.scale,
                .w = player.scale,
                .h = player.scale
            };

            if (board.array[i][j] == 1) temp = bBrick;
            else if (board.array[i][j] == 2) temp = bBrickWindow;
            else temp = bBrickFloor;

            SDL_RenderTexture(renderer, temp, NULL, &dst);
        }
    }
}

void drawPlayer(SDL_Renderer *renderer)
{
    setColor(renderer, red);
    drawRectOffset(renderer, player.board.x, player.board.y, player.scale * 0.25, player.scale * 0.25);
    setColor(renderer, black);
    drawRectOffset(renderer, (player.pos.x + player.dir.x) * player.scale, (player.pos.y + player.dir.y) * player.scale, player.scale * 0.1, player.scale * 0.1);
}

void renderClear(SDL_Renderer *renderer, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
    return;
}

#endif // LIB_DRAW_H_
