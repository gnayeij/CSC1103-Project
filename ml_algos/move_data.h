#ifndef MOVE_DATA_H
#define MOVE_DATA_H

#include <stddef.h>

#define BOARD_SIZE 9
// Define the dataset structure
typedef struct movedata
{
    char board[3][3]; // player 1 is 'X', player 2 is 'O', blank is 'b'
    int outcome;      //
} MoveData;
#endif
