#ifndef LIB_BOARD_H_
#define LIB_BOARD_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Board
{
    uint8_t **array;

    int width;
    int height;
    int scale;
} Board;

bool createBoard(Board *board, int width, int height, int scale)
{
    board->array = malloc(sizeof(uint8_t *) * height);
    if (board->array == NULL) { printf("malloc error\n"); return false; }

    for (int i = 0; i < height; i++)
    {
        board->array[i] = malloc(sizeof(uint8_t) * width);
        if (board->array[i] == NULL) { printf("malloc error [%i]\n", i); return false; }
    }

    board->width = width;
    board->height = height;
    board->scale = scale;

    return true;
}

void setBoard(Board *board)
{
    for (int i = 0; i < board->height; i++)
    {
        for (int j = 0; j < board->width; j++)
        {
            if (i == 0 || i == board->height - 1 || j == 0 || j == board->width - 1)
                board->array[i][j] = 1;
            else board->array[i][j] = 0;
        }
    }
}

void updateBoard(Board *board, int x, int y, int value)
{
    board->array[y][x] = value;
}

void destroyBoard(Board *board)
{
    for (int i = 0; i < board->height; i++)
    {
        free(board->array[i]);
    }

    free(board->array);
}

#endif // LIB_BOARD_H_
