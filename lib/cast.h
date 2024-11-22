#ifndef LIB_CAST_H_
#define LIB_CAST_H_

#include "board.h"
#include "point.h"
#include "start.h"

#include <stdint.h>
#include <math.h>

typedef struct Cast
{
    Point hit;
    double distance;
    double factor;
    int index;
    uint8_t type;
    bool side;
} Cast;

typedef struct Sprite
{
    Point pos;
    int type;
} Sprite;

Sprite sprite[SN] =
{
    {.pos = { .x = 5.5, .y = 5.5 }, .type = LAMP},
    {.pos = { .x = 6.5, .y = 6.5 }, .type = CROUCH},
    {.pos = { .x = 7.5, .y = 7.5 }, .type = GUY}
};
Sprite sortedSprite[SN];

uint8_t hitMap[BH][BW];
double distances[STEPS];

uint8_t hit(Point position, Point direction)
{
    bool isXWhole = false;
    if (position.x == floor(position.x)) isXWhole = true;

    int x = floor(position.x);
    int y = floor(position.y);

    if (x < 0 || x > board.width - 1 || y < 0 || y > board.height - 1) return 1;

    if (direction.x < 0 && isXWhole) x--;
    if (direction.y < 0 && !isXWhole) y--;

    hitMap[y][x] = 1;

    return board.array[y][x];
}

Cast cast(double angle)
{
    Cast out;

    Point rayDirection = { .x = cos(angle), .y = sin(angle) };

    double slope = rayDirection.y / rayDirection.x;
    double intercept = (player.pos.y + rayDirection.y) - (slope * (player.pos.x + rayDirection.x)); // b = y - mx

    Point hitX;
    if (rayDirection.x > 0) hitX.x = floor(player.pos.x + 1); else hitX.x = floor(player.pos.x);
    hitX.y = slope * hitX.x + intercept; // y = mx + b

    Point hitY;
    if (rayDirection.y > 0) hitY.y = floor(player.pos.y + 1); else hitY.y = floor(player.pos.y);
    hitY.x = (hitY.y - intercept) / slope; // x = (y - b) / m
    
    double deltaDistanceX = tan((M_PI / 2) - angle);
    double deltaDistanceY = tan(angle);

    uint8_t typeX, typeY;

    while (!(typeX = hit(hitX, rayDirection)))
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
    while (!(typeY = hit(hitY, rayDirection)))
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

    double f = cos(angle - player.angle);

    if (hitXDistance < hitYDistance)
    {
        out.hit = hitX;
        out.distance = hitXDistance * f;
        out.factor = f;
        out.index = (hitX.y - floor(hitX.y)) * TS;
        out.type = typeX;
        out.side = true;
    }
    else 
    {
        out.hit = hitY;
        out.distance = hitYDistance * f;
        out.factor = f;
        out.index = (hitY.x - floor(hitY.x)) * TS;
        out.type = typeY;
        out.side = false;
    }

    return out;
}

void raycast(int steps)
{
    double startAngle = player.angle - (FOV * 0.5);
    double stepAngle = FOV / steps;
    int rectWidth = (SWW / steps); 
    double rectHeight;

    // Clear hitmap
    for (int i = 0; i < BH; i++)
    {
        for (int j = 0; j < BW; j++)
        {
            hitMap[i][j] = 0;
        }
    }

    for (int i = 0; i < steps; i++)
    {
        Cast out = cast(startAngle);

        // Floor casting
        double screenMiddle = SWH * 0.5;

        double initialAngle = SDL_atan(0.5);
        double endAngle = SDL_atan((1.0 / SWH));
        Point pointForWall = { .x = SDL_cos(startAngle), .y = SDL_sin(startAngle) };

        double verticalHalfSteps = screenMiddle / rectWidth;

        double angleStep = SDL_fabs(initialAngle - endAngle) / verticalHalfSteps;

        for (int j = SWH; j > screenMiddle + 1; j -= rectWidth)
        {
            double distanceFromScreenMiddle = j - screenMiddle;

            double anotherAngle = ND - initialAngle;

            // SDL_tan(anotherAngle) = 1 / distanceFromScreenMiddle;
            // SDL_tan(anotherAngle) = X / 0.5

            double distFromFloor = SDL_tan(anotherAngle) * 0.5 * 0.95;

            // Fisheye correction
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

            SDL_FRect fdst =
            {
                .x = i * rectWidth,
                .y = j,
                .w = rectWidth,
                .h = rectWidth
            };

            SDL_FRect cdst =
            {
                .x = i * rectWidth,
                .y = screenMiddle - (j - screenMiddle),
                .w = rectWidth,
                .h = rectWidth
            };


            SDL_RenderTexture(sRender, sTexture[FLOOR], &src, &fdst);
            // Comment/uncomment this line to render ceilings
            // SDL_RenderTexture(sRender, sTexture[FLOOR], &src, &cdst);

            initialAngle -= angleStep;
        }
        
        // Wall cast
        rectHeight = SWH / out.distance;

        distances[i] = out.distance;

        SDL_FRect src =
        {
            .x = out.index,
            .y = 0,
            .w = 1,
            .h = TS
        };

        SDL_FRect dst =
        {
            .x = i * rectWidth,
            .y = (SWH * 0.5) - (rectHeight * 0.5),
            .w = rectWidth,
            .h = rectHeight
        };
        
        SDL_Texture *texture;

        texture = sTexture[out.type];

        SDL_RenderTexture(sRender, texture, &src, &dst);

        if (out.side)
        {
            setColor(sRender, gray);
            SDL_RenderFillRect(sRender, &dst);
        }

        setColor(bRender, light);
        SDL_RenderLine(bRender, player.pos.x * board.scale, player.pos.y * board.scale, out.hit.x * board.scale, out.hit.y * board.scale);
        startAngle += stepAngle;
    }
}

void pang(double angle)
{
    printf("%f\n", angle / (M_PI / 180));
}

void drawPoint(Point a)
{
    SDL_FRect rect =
    {
        .x = a.x * board.scale - 2,
        .y = a.y * board.scale - 2,
        .w = 4,
        .h = 4
    };

    setColor(bRender, white);
    SDL_RenderFillRect(bRender, &rect);
}

void sortSprites(void)
{
    Sprite sSprites[SN];

    double far = 10000;

    double dist[SN];
    
    // Calculate distances of sprites
    for (int i = 0; i < SN; i++)
    {
        dist[i] = distanceBetweenPointsPerp(player.pos, player.dir, sprite[i].pos);
    }

    for (int i = 0; i < SN; i++)
    {
        // Iterate through distances
        // Store lowest distance
        // After loop, change lowest distance to far
        int lowest = 0;
        for (int j = 0; j < SN; j++)
        {
            if (dist[lowest] > dist[j]) lowest = j;
        }

        sortedSprite[i] = sprite[lowest];
        dist[lowest] = far;
    }    
}

void drawSprite(int steps)
{
    for (int i = SN - 1; i > -1; i--)
    {
        int sptype = sortedSprite[i].type;

        // Sprite minus point
        Point smp = subPoints(sortedSprite[i].pos, player.pos);

        // How much do we have to rotate the player to be 90 degrees?
        double diff = (M_PI * 0.5) - player.angle;

        // Rotate around player so player is facing up
        smp = rotatePoint(smp, -diff);
        Point dirRot = rotatePoint(player.dir, -diff);

        double smpA = atan2(smp.y, smp.x);
        double smpP = atan2(dirRot.y, dirRot.x);
        double smpL = smpP - (FOV * 0.5);

        Point smpLPoint = unitVector(smpL);

        int index = ((smpA - smpL) / FOV) * SWW;
        double sDist = distanceBetweenPointsPerp(player.pos, player.dir, sortedSprite[i].pos);
        int height = SWH / sDist;

        // Dont draw sprite if too close to player
        if (sDist > 1)
        {
            int stepWidth = SWW / steps;

            int spriteStartX = SWW - index - (height * 0.5);
            spriteStartX = (spriteStartX / stepWidth) * stepWidth;

            int spriteWidth = height / stepWidth;

            int startStep = spriteStartX / stepWidth;

            for (int i = 0; i < spriteWidth; i++)
            {
                int spriteXPos = spriteStartX + (i * stepWidth);

                if (distances[startStep + i] > sDist)
                {
                    SDL_FRect src =
                    {
                        .x = ((double)i / spriteWidth) * TS,
                        .y = 0,
                        .w = 1,
                        .h = TS
                    };

                    SDL_FRect dst =
                    {
                        .x = spriteXPos,
                        .y = SWH * 0.5 - height * 0.5,
                        .w = stepWidth,
                        .h = height
                    };

                    SDL_RenderTexture(sRender, sTexture[sptype], &src, &dst);
                }
            }
        }
    }
}

#endif // LIB_CAST_H_
