#ifndef LIB_CAST_H_
#define LIB_CAST_H_

#include "start.h"
#include "board.h"
#include "point.h"
#include "draw.h"

#include <SDL3/SDL.h>

#include <math.h>

const double ND = M_PI * 0.5;
const double FOV = 1.15192;

typedef struct Cast
{
    Point hit;
    double distance;
    int index;
    bool side;
    uint8_t type;
    Point dir;
} Cast;

Cast cast(double angle);
void raycast(double steps);
uint8_t hit(Point position, Point direction, Board board);
void floorCast(int fSize, double end);

Cast cast(double angle)
{
    Cast output;

    Point rayDirection = { .x = SDL_cos(angle), .y = SDL_sin(angle) };

    double slope = rayDirection.y / rayDirection.x;
    double intercept = (player.pos.y + rayDirection.y) - (slope * (player.pos.x + rayDirection.x)); // b = y - mx

    Point hitX;
    if (rayDirection.x > 0) hitX.x = SDL_floor(player.pos.x + 1); else hitX.x = SDL_floor(player.pos.x);
    hitX.y = slope * hitX.x + intercept; // y = mx + b

    Point hitY;
    if (rayDirection.y > 0) hitY.y = SDL_floor(player.pos.y + 1); else hitY.y = SDL_floor(player.pos.y);
    hitY.x = (hitY.y - intercept) / slope; // x = (y - b) / m
    
    double deltaDistanceX = SDL_tan(ND - angle);
    double deltaDistanceY = SDL_tan(angle);

    while (!hit(hitX, rayDirection, board))
    {
        if (rayDirection.x > 0)
        {
            hitX.x++;
            hitX.y += deltaDistanceY;
        }
        else
        {
            hitX.x--;
            hitX.y -= deltaDistanceY;
        }
    }
    while (!hit(hitY, rayDirection, board))
    {
        if (rayDirection.y > 0)
        {
            hitY.y++;
            hitY.x += deltaDistanceX;
        }
        else
        {
            hitY.y--;
            hitY.x -= deltaDistanceX;
        }
    }

    double hitXDistance = distanceBetweenPoints(player.pos, hitX);
    double hitYDistance = distanceBetweenPoints(player.pos, hitY);

    if (hitXDistance < hitYDistance)
    {
        output.hit = hitX;
        output.distance = hitXDistance * SDL_cos(angle - player.angle);
        output.index = (hitX.y - SDL_floor(hitX.y)) * TS;
        output.side = true;
        output.dir = rayDirection;
    }
    else 
    {
        output.hit = hitY;
        output.distance = hitYDistance * SDL_cos(angle - player.angle);
        output.index = (hitY.x - SDL_floor(hitY.x)) * TS;
        output.side = false;
        output.dir = rayDirection;
    }

    return output;
}

void raycast(double steps)
{
    double startAngle = player.angle - (FOV * 0.5);
    double stepAngle = FOV / steps;
    int rectWidth = SDL_ceil((double)SWW / steps); 
    double rectHeight;

    double endAngle = startAngle + (stepAngle * (steps));

    floorCast(rectWidth, endAngle);

    setColor(boardRenderer, red);
    for (int i = 0; i < steps; i++)
    {
        Cast out = cast(startAngle);

        // Wall cast
        setColor(boardRenderer, red);
        rectHeight = SWH / out.distance;

        SDL_FRect rect =
        {
            .x = i * rectWidth,
            .y = (SWH * 0.5) - (rectHeight * 0.5),
            .w = rectWidth,
            .h = rectHeight
        };

        SDL_FRect srcRect = getIndexRect(out.index);

        SDL_RenderTexture(screenRenderer, sBrick, &srcRect, &rect);

        if (out.side)
        {
            setColor(screenRenderer, gray);
            SDL_RenderFillRect(screenRenderer, &rect);
        }

        SDL_RenderLine(boardRenderer, player.pos.x * player.scale, player.pos.y * player.scale, out.hit.x * player.scale, out.hit.y * player.scale);

        startAngle += stepAngle;
    }
}

uint8_t hit(Point position, Point direction, Board board)
{
    bool isXWhole = false;
    if (position.x == SDL_floor(position.x)) isXWhole = true;

    int x = SDL_floor(position.x);
    int y = SDL_floor(position.y);

    if (x < 0 || x > board.width - 1 || y < 0 || y > board.height - 1) return 1;

    if (direction.x < 0 && isXWhole) x--;
    if (direction.y < 0 && !isXWhole) y--;
    return board.array[y][x];
}

void floorCast(int fSize, double end)
{
    double playerHeight = 0.5;
    double cameraHeight = 1.0;
    double cameraDistance = 1.0;
    double cameraMiddle = 0.5;
    double verticalCameraStep = (1.0 / SWH) * fSize;
    
    Point left = unitVector((player.angle - (FOV * 0.5)));
    Point right = unitVector(end);

    Point lHit, rHit;

    double dx, dy;

    int xIndex, yIndex;

    SDL_FRect src, dst;

    for (double i = 0; i < cameraMiddle; i += verticalCameraStep)
    {
        double angleA = SDL_atan(i / cameraDistance);
        double angleB = ND - angleA;

        // tan(angleB) = cameraDistance / i;
        // tan(angleB) = distanceToFLoor / playerHeight;
        // tan(angleB) * playerHeight = distanceToFloor;

        double distanceToFloorHit = SDL_tan(angleB) * playerHeight * 1.3;

        lHit = (Point)
        {
            .x = player.pos.x + (left.x * distanceToFloorHit),
            .y = player.pos.y + (left.y * distanceToFloorHit)
        };

        rHit = (Point)
        {
            .x = player.pos.x + (right.x * distanceToFloorHit),
            .y = player.pos.y + (right.y * distanceToFloorHit)
        };

        Point lPre = lHit;

        dx = (rHit.x - lHit.x) / SWW * fSize;
        dy = (rHit.y - lHit.y) / SWW * fSize;

        for (int j = 0; j < SWW; j += fSize)
        {
            xIndex = (lHit.x - SDL_floor(lHit.x)) * TS;
            yIndex = (lHit.y - SDL_floor(lHit.y)) * TS;
            
            src = (SDL_FRect)
            {
                .x = xIndex,
                .y = yIndex,
                .w = 1,
                .h = 1
            };

            dst = (SDL_FRect)
            {
                .x = j,
                .y = (SWH * 0.5) + (i * SWH),
                .w = fSize,
                .h = fSize
            };

            SDL_RenderTexture(screenRenderer, sBrickFloor, &src, &dst);

            lHit.x += dx;
            lHit.y += dy;
        }

        Point lPost = lHit;

        setColor(boardRenderer, white);
        SDL_RenderLine(boardRenderer, lPre.x * player.scale, lPre.y * player.scale, lPost.x * player.scale, lPost.y * player.scale);

    }

}

#endif // LIB_CAST_H_

