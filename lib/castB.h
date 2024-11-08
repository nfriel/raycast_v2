#ifndef LIB_CAST_H_
#define LIB_CAST_H_

#include "start.h"
#include "board.h"
#include "point.h"
#include "draw.h"

#include <SDL3/SDL.h>

#define ND 1.571
#define FOV 1.15192

typedef struct Cast
{
    Point hit;
    Point dir;
    double distance;
    double factor;
    int index;
    bool side;
    uint8_t type;
} Cast;

uint8_t hit(Point position, Point direction);
Cast cast(double angle);
void raycast(double steps);

uint8_t hit(Point position, Point direction)
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

Cast cast(double angle)
{
    Cast out;

    Point rayDirection = unitVector(angle);

    double slope = SDL_tan(angle);
    double intercept = (player.pos.y + rayDirection.y) - (slope * (player.pos.x + rayDirection.x)); // b = y - mx

    Point hitX;
    if (rayDirection.x > 0) hitX.x = SDL_floor(player.pos.x + 1); else hitX.x = SDL_floor(player.pos.x);
    hitX.y = slope * hitX.x + intercept; // y = mx + b

    Point hitY;
    if (rayDirection.y > 0) hitY.y = SDL_floor(player.pos.y + 1); else hitY.y = SDL_floor(player.pos.y);
    hitY.x = (hitY.y - intercept) / slope; // x = (y - b) / m
    
    double deltaDistanceX = SDL_tan(ND - angle);
    double deltaDistanceY = SDL_tan(angle);

    uint8_t hitXType, hitYType;

    while (!(hitXType = hit(hitX, rayDirection)))
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
    while (!(hitYType = hit(hitY, rayDirection)))
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

    double f = SDL_cos(angle - player.angle);

    if (hitXDistance < hitYDistance)
    {
        out.hit = hitX;
        out.distance = hitXDistance * f;
        out.index = (hitX.y - SDL_floor(hitX.y)) * TS;
        out.side = true;
        out.dir = rayDirection;
        out.factor = f;
        out.type = hitXType;
    }
    else 
    {
        out.hit = hitY;
        out.distance = hitYDistance * f;
        out.index = (hitY.x - SDL_floor(hitY.x)) * TS;
        out.side = false;
        out.dir = rayDirection;
        out.factor = f;
        out.type = hitYType;
    }

    return out;
}

void raycast(double steps)
{
    double startAngle = player.angle - (FOV * 0.5);
    double stepAngle = FOV / steps;
    int rectWidth = SDL_ceil((double)SWW / steps); 
    double rectHeight;

    setColor(boardRenderer, red);
    for (int i = 0; i < steps; i++)
    {
        Cast out = cast(startAngle);
        // Floor casting

        double screenMiddle = SWH * 0.5;
        double playerZPos = 0.5;
        double distanceFromCamera = 1.0;
        double distanceFromScreenMiddle;

        double initialAngle = SDL_atan(0.5);
        double endAngle = SDL_atan((1.0 / SWH));
        Point pointForWall = { .x = SDL_cos(startAngle), .y = SDL_sin(startAngle) };

        double verticalHalfSteps = screenMiddle / rectWidth;

        double angleStep = SDL_fabs(initialAngle - endAngle) / verticalHalfSteps;

        for (int j = SWH; j > screenMiddle + 1; j -= rectWidth)
        {
            distanceFromScreenMiddle = j - screenMiddle;

            double anotherAngle = ND - initialAngle;

            // SDL_tan(anotherAngle) = 1 / distanceFromScreenMiddle;
            // SDL_tan(anotherAngle) = X / 0.5

            double distFromFloor = SDL_tan(anotherAngle) * 0.5 * 0.95;
            distFromFloor *= 1 / out.factor;

            if (distFromFloor > 40) break;

            Point floorHit = { .x = player.pos.x + (pointForWall.x * distFromFloor), .y = player.pos.y + (pointForWall.y * distFromFloor) };

            int floorXIndex = (floorHit.x - SDL_floor(floorHit.x)) * TS;
            int floorYIndex = (floorHit.y - SDL_floor(floorHit.y)) * TS;

            SDL_FRect src = 
            {
                .x = floorXIndex,
                .y = floorYIndex,
                .w = 1,
                .h = 1
            };

            SDL_FRect floorDrawRect =
            {
                .x = i * rectWidth,
                .y = j,
                .w = rectWidth,
                .h = rectWidth
            };

            SDL_RenderTexture(screenRenderer, sBrickFloor, &src, &floorDrawRect);

            initialAngle -= angleStep;
        }
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

        
        SDL_Texture *texture;

        if (out.type == 1) texture = sBrick;
        if (out.type == 2) texture = sBrickWindow;

        SDL_RenderTexture(screenRenderer, texture, &srcRect, &rect);

        if (out.side)
        {
            setColor(screenRenderer, gray);
            SDL_RenderFillRect(screenRenderer, &rect);
        }

        SDL_RenderLine(boardRenderer, player.pos.x * player.scale, player.pos.y * player.scale, out.hit.x * player.scale, out.hit.y * player.scale);
        startAngle += stepAngle;
    }
}

#endif // LIB_CAST_H_

