#ifndef LIB_BOARD_H_
#define LIB_BOARD_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define BW 20     // Board width
#define BH 20     // Board height

typedef struct Board
{
    uint8_t **array;
    int width;
    int height;
} Board;

Board board;

bool createBoard(void);
void setBoard(void);
void updateBoard(int x, int y, uint8_t add);
void destroyBoard(void);

bool createBoard(void)
{
    board.width = BW;
    board.height = BH;

    board.array = malloc(sizeof(uint8_t*) * BH);
    if (board.array == NULL)
    {
        printf("malloc error 0\n");
        return false;
    }

    for (int i = 0; i < BH; i++)
    {
        board.array[i] = malloc(sizeof(uint8_t) * BW);
        if (board.array[i] == NULL)
        {
            printf("malloc error [%i]\n", i);
            return false;
        }
    }
    return true;
}

void setBoard(void)
{
    for (int i = 0; i < BH; i++)
    {
        for (int j = 0; j < BW; j++)
        {
            if (j == 0 || j == BW - 1 || i == 0 || i == BH - 1)
            {
                board.array[i][j] = 1;
            }
            else
            {
                board.array[i][j] = 0;
            }
        }
    }
}

void updateBoard(int x, int y, uint8_t add)
{
    if (add) board.array[y][x] = add;
    else board.array[y][x] = 0;
}

void destroyBoard(void)
{
    for (int i = 0; i < BW; i++)
    {
        free(board.array[i]);
    }
    free(board.array);
}

#endif // LIB_BOARD_H_
