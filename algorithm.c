#include <stdio.h>
#include <stdbool.h>

#define BOARD_SIZE 3

// base algorithm for tic tac toe 

void initializeBoard(char board[BOARD_SIZE][BOARD_SIZE]);
void printBoard(char board[BOARD_SIZE][BOARD_SIZE]);
bool makeMove(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, char player);
bool checkWin(char board[BOARD_SIZE][BOARD_SIZE], char player);
bool isBoardFull(char board[BOARD_SIZE][BOARD_SIZE]);

int main() {
    char board[BOARD_SIZE][BOARD_SIZE];
    char currentPlayer = 'X';
    bool gameWon = false;
    int row, col;

    initializeBoard(board);

    while (!gameWon && !isBoardFull(board)) {
        printBoard(board);
        printf("Player %c's turn. Enter row (0-2) and column (0-2): ", currentPlayer);
        scanf("%d %d", &row, &col);

        if (makeMove(board, row, col, currentPlayer)) {
            gameWon = checkWin(board, currentPlayer);
            if (!gameWon) {
                currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
            }
        } else {
            printf("Invalid move. Try again.\n");
        }
    }

    printBoard(board);
    if (gameWon) {
        printf("Player %c wins!\n", currentPlayer);
    } else {
        printf("It's a draw!\n");
    }

    return 0;
}

void initializeBoard(char board[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = ' ';
        }
    }
}

void printBoard(char board[BOARD_SIZE][BOARD_SIZE]) {
    printf("\n");
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf(" %c ", board[i][j]);
            if (j < BOARD_SIZE - 1) printf("|");
        }
        printf("\n");
        if (i < BOARD_SIZE - 1) printf("---+---+---\n");
    }
    printf("\n");
}

bool makeMove(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, char player) {
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE || board[row][col] != ' ') {
        return false;
    }
    board[row][col] = player;
    return true;
}

bool checkWin(char board[BOARD_SIZE][BOARD_SIZE], char player) {
    // check rows, columns, and diagonals
    for (int i = 0; i < BOARD_SIZE; i++) {
        if ((board[i][0] == player && board[i][1] == player && board[i][2] == player) ||
            (board[0][i] == player && board[1][i] == player && board[2][i] == player)) {
            return true;
        }
    }
    if ((board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
        (board[0][2] == player && board[1][1] == player && board[2][0] == player)) {
        return true;
    }
    return false;
}

bool isBoardFull(char board[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == ' ') {
                return false;
            }
        }
    }
    return true;
}