#ifndef LIB_POINT_H_
#define LIB_POINT_H_

#include <SDL3/SDL.h>

#include <math.h>

typedef struct Point
{
    double x;
    double y;
} Point;

typedef struct Player
{
    Point pos;
    Point dir;
    Point board;
    int scale;
    double angle;
} Player;

Player player;

void setPlayer(int bw, int bh, int bsw, int bsh);
void updatePlayer(void);
Point addPoints(Point a, Point b);
Point subPoints(Point a, Point b);
double distanceBetweenPoints(Point a, Point b);
double distanceBetweenPointsPerp(Point a, Point b, Point c);
Point scalePoint(Point a, double scale);
Point unitVector(double angle);


void setPlayer(int bw, int bh, int bsw, int bsh)
{
    player.pos = (Point)
    {
        .x = bw * 0.5,
        .y = bh * 0.5
    };

    player.dir = (Point)
    {
        .x = 1,
        .y = 0
    };

    player.board = (Point)
    {
        .x = bsw * 0.5,
        .y = bsw * 0.5
    };

    player.angle = 0;
    player.scale = bsw / bw;
}

void updatePlayer(void)
{
    player.dir = (Point)
    {
        .x = SDL_cos(player.angle),
        .y = SDL_sin(player.angle)
    };

    player.board = (Point)
    {
        .x = player.pos.x * player.scale,
        .y = player.pos.y * player.scale
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
    double x = fabs(b.x - a.x);
    double y = fabs(b.y - a.y);
    return sqrt((x * x) + (y * y));
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

    double distance = fabs((A * c.x) + (B * c.y) + C) / sqrt(A * A + B * B);

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

#endif // LIB_POINT_H_
