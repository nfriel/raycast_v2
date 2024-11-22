#ifndef LIB_DRAW_H_
#define LIB_DRAW_H_

#include "board.h"
#include "head.h"
#include "point.h"

#include <SDL3/SDL.h>

#include <stdint.h>

enum 
{
    FLOOR,
    BRICK,
    WINDOW,
    PLAYER,
    EYE,
    GUY,
    CROUCH,
    LAMP,
    HANDS,
    TNUM
};

SDL_Color white = { 255, 255, 255, 255 };
SDL_Color black = { 0, 0, 0, 255 };
SDL_Color red = { 255, 0, 0, 255 };
SDL_Color green = { 0, 255, 0, 255 };
SDL_Color blue = { 0, 0, 255, 255 };
SDL_Color gray = { 0, 0, 0, 100 };
SDL_Color light = { 255, 255, 255, 100};

void setColor(SDL_Renderer *renderer, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.g, color.g, color.b, color.a);
}

bool loadImages(SDL_Renderer *rendererA, SDL_Renderer *rendererB, SDL_Texture **textureA, SDL_Texture **textureB)
{
    SDL_Surface *surface[TNUM];

    surface[BRICK] = SDL_LoadBMP("assets/brick.bmp");
    if (surface[BRICK] == NULL) return error("SDL_LoadBMP brick");
    
    surface[WINDOW]= SDL_LoadBMP("assets/brickWindow.bmp");
    if (surface[WINDOW]  == NULL) return error("SDL_LoadBMP window");

    surface[FLOOR] = SDL_LoadBMP("assets/brickFloor.bmp");
    if (surface[FLOOR] == NULL) return error("SDL_LoadBMP floor");

    surface[PLAYER] = SDL_LoadBMP("assets/player.bmp");
    if (surface[PLAYER] == NULL) return error("SDL_LoadBMP player");

    surface[EYE] = SDL_LoadBMP("assets/eye.bmp");
    if (surface[EYE] == NULL) return error("SDL_LoadBMP eye");

    surface[GUY] = SDL_LoadBMP("assets/guy.bmp");
    if (surface[GUY] == NULL) return error("SDL_LoadBMP guy");

    surface[CROUCH] = SDL_LoadBMP("assets/crouch.bmp");
    if (surface[CROUCH] == NULL) return error("SDL_LoadBMP crouch");

    surface[LAMP] = SDL_LoadBMP("assets/lamp.bmp");
    if (surface[LAMP] == NULL) return error("SDL_LoadBMP lamp");

    surface[HANDS] = SDL_LoadBMP("assets/hands.bmp");
    if (surface[HANDS] == NULL) return error("SDL_LoadBMP hands");

    for (int i = 0; i < TNUM; i++)
    {
        textureA[i] = SDL_CreateTextureFromSurface(rendererA, surface[i]);
        textureB[i] = SDL_CreateTextureFromSurface(rendererB, surface[i]);

        SDL_DestroySurface(surface[i]);

        if (textureA[i] == NULL) return error("SDL_CreateTextureFromSurface");
        if (textureB[i] == NULL) return error("SDL_CreateTextureFromSurface");

        // Prevent blurry upscaling
        SDL_SetTextureScaleMode(textureA[i], SDL_SCALEMODE_NEAREST);
        SDL_SetTextureScaleMode(textureB[i], SDL_SCALEMODE_NEAREST);

        SDL_SetRenderDrawBlendMode(rendererA, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawBlendMode(rendererB, SDL_BLENDMODE_BLEND);

    }

    

    return true;
}

void unloadImages(SDL_Texture **textureA, SDL_Texture **textureB)
{
    for (int i = 0; i < TNUM; i++)
    {
        SDL_DestroyTexture(textureA[i]);
        SDL_DestroyTexture(textureB[i]);
    }
}

void drawBoard(SDL_Renderer *renderer, SDL_Texture **texture, Board board)
{
    for (int i = 0; i < board.height; i++)
    {
        for (int j = 0; j < board.width; j++)
        {
            SDL_FRect rect =
            {
                .x = j * board.scale,
                .y = i * board.scale,
                .w = board.scale,
                .h = board.scale
            };

            if (!SDL_RenderTexture(renderer, texture[board.array[i][j]], NULL, &rect))
                error("SDL_RenderTexture");
        }
    }
}

void drawPlayer(SDL_Renderer *renderer, SDL_Texture **texture, Board board, Player player)
{
    SDL_FRect pRect =
    {
        .x = (player.pos.x * board.scale) - (board.scale * 0.5),
        .y = (player.pos.y * board.scale) - (board.scale * 0.5),
        .w = board.scale,
        .h = board.scale
    };

    SDL_FRect dRect =
    {
        .x = ((player.pos.x + player.dir.x) * board.scale) - (board.scale * 0.5),
        .y = ((player.pos.y + player.dir.y) * board.scale) - (board.scale * 0.5),
        .w = board.scale,
        .h = board.scale
    };

    SDL_RenderTexture(renderer, texture[PLAYER], NULL, &pRect);
    SDL_RenderTexture(renderer, texture[EYE], NULL, &dRect);
}

void drawHands(SDL_Renderer *renderer, SDL_Texture **texture)
{
    SDL_RenderTexture(renderer, texture[HANDS], NULL, NULL);
}

#endif // LIB_DRAW_H_
