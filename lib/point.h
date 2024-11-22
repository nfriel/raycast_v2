#ifndef LIB_POINT_H_
#define LIB_POINT_H_

#include <stdint.h>
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
        .x = cos(player->angle),
        .y = sin(player->angle)
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

    double distance = fabs((A * c.x) + (B * c.y) + C) / sqrt((A * A) + (B * B));

    return distance;
}

Point scalePoint(Point a, double scale)
{
    return (Point) { .x = a.x * scale, .y = a.y * scale };
}

Point unitVector(double angle)
{
    return (Point) { .x = cos(angle), .y = sin(angle) };
}

Point normalizePoint(Point a)
{
    double magnitude = sqrt((a.x * a.x) + (a.y * a.y));

    return (Point) { .x = a.x / magnitude, .y = a.y / magnitude };
}

Point rotatePoint(Point a, double angle)
{
    Point temp = a;

    temp.x = a.x * cos(angle) + a.y * sin(angle);
    temp.y = a.x * sin(angle) - a.y * cos(angle);

    return temp;
}

#endif // LIB_POINT_H_
