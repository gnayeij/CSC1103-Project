#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BOARD_SIZE 3

// base algorithm for tic tac toe

void initializeBoard(char board[BOARD_SIZE][BOARD_SIZE]);
void printBoard(char board[BOARD_SIZE][BOARD_SIZE]);
bool makeMove(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, char player);
bool checkWin(char board[BOARD_SIZE][BOARD_SIZE], char player);
bool isBoardFull(char board[BOARD_SIZE][BOARD_SIZE]);

int main()
{

    char board[BOARD_SIZE][BOARD_SIZE];
    char currentPlayer = 'X';
    bool gameWon = false;
    int row, col;
    FILE *file_ptr;
    if ((file_ptr = fopen("tic-tac-toe.txt", "r")) == NULL)
    {
        printf("File could not be opened \n");
        exit(1);
    }

    char a, b, c, d, e, f, g, h, i;
    fscanf(file_ptr, "%c %c %c %c %c %c %c %c %c \n", &a, &b, &c, &d, &e, &f, &g, &h, &i);
    printf("%c %c %c %c %c %c %c %c %c \n", a, b, c, d, e, f, g, h, i);

    fclose(file_ptr);

    initializeBoard(board);

    printBoard(board);
}

void initializeBoard(char board[BOARD_SIZE][BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            board[i][j] = ' ';
        }
    }
}

void printBoard(char board[BOARD_SIZE][BOARD_SIZE])
{
    printf("\n");
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            printf(" %c ", board[i][j]);
            if (j < BOARD_SIZE - 1)
                printf("|");
        }
        printf("\n");
        if (i < BOARD_SIZE - 1)
            printf("---+---+---\n");
    }
    printf("\n");
}
