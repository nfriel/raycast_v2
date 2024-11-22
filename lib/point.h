#ifndef LIB_POINT_H_
#define LIB_POINT_H_

#include <SDL3/SDL.h>

#include <stdint.h>

typedef struct Point
{
    double x;
    double y;
} Point;

typedef struct Player
{
    Point pos;
    Point dir;
    double angle;
} Player;

void placePlayer(Player *player, int width, int height)
{
    player->pos.x = width * 0.5;
    player->pos.y = height * 0.5;
    player->dir.x = 1;
    player->dir.y = 0;
    player->angle = 0;
}

void updatePlayer(Player *player)
{
    player->dir = (Point)
    {
        .x = SDL_cos(player->angle),
        .y = SDL_sin(player->angle)
    };
}

Point addPoints(Point a, Point b)
{
    return (Point) { .x = a.x + b.x, .y = a.y + b.y };
}

Point subPoints(Point a, Point b)
{
    return (Point) { .x = a.x - b.x, .y = a.y - b.y };
}

double distanceBetweenPoints(Point a, Point b)
{
    double x = SDL_fabs(b.x - a.x);
    double y = SDL_fabs(b.y - a.y);
    return SDL_sqrt((x * x) + (y * y));
}

double distanceBetweenPointsPerp(Point a, Point b, Point c) // player, vector, hit
{
    Point camPlane = { .x = b.y, .y = b.x * -1 };

    double x1 = a.x;
    double y1 = a.y;
    double x2 = a.x + camPlane.x;
    double y2 = a.y + camPlane.y;

    double A = y1 - y2;
    double B = x2 - x1;
    double C = (x1 - x2) * y1 + (y2 - y1) * x1;

    double distance = SDL_fabs((A * c.x) + (B * c.y) + C) / SDL_sqrt((A * A) + (B * B));

    return distance;
}

Point scalePoint(Point a, double scale)
{
    return (Point) { .x = a.x * scale, .y = a.y * scale };
}

Point unitVector(double angle)
{
    return (Point) { .x = SDL_cos(angle), .y = SDL_sin(angle) };
}

Point normalizePoint(Point a)
{
    double magnitude = SDL_sqrt((a.x * a.x) + (a.y * a.y));

    return (Point) { .x = a.x / magnitude, .y = a.y / magnitude };
}

Point rotatePoint(Point a, double angle)
{
    Point temp = a;

    temp.x = a.x * SDL_cos(angle) + a.y * SDL_sin(angle);
    temp.y = a.x * SDL_sin(angle) - a.y * SDL_cos(angle);

    return temp;
}

#endif // LIB_POINT_H_
