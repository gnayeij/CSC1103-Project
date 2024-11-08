#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 3
#define EMPTY -1
#define PLAYER_X 1
#define PLAYER_O 0

// Function prototypes
void printBoard(int board[SIZE][SIZE]);
int checkWin(int board[SIZE][SIZE]);
int minimax(int board[SIZE][SIZE], int depth, int isMaximizingPlayer, int alpha, int beta);
int isMovesLeft(int board[SIZE][SIZE]);
int evaluate(int board[SIZE][SIZE]);
void aiMove(int board[SIZE][SIZE], int player);
int randomMove(int board[SIZE][SIZE]);

// Function to print the Tic-Tac-Toe board
void printBoard(int board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == PLAYER_X)
                printf(" X ");
            else if (board[i][j] == PLAYER_O)
                printf(" O ");
            else
                printf(" - ");
            
            if (j < SIZE - 1) printf("|");
        }
        printf("\n");
        if (i < SIZE - 1) printf("-----------\n");
    }
    printf("\n");
}

// Function to check if there is a winner
int checkWin(int board[SIZE][SIZE]) {
    // Check rows for a win
    for (int row = 0; row < SIZE; row++) {
        if (board[row][0] == board[row][1] && board[row][1] == board[row][2]) {
            if (board[row][0] == PLAYER_X) return PLAYER_X;  // Player X wins
            if (board[row][0] == PLAYER_O) return PLAYER_O;  // Player O wins
        }
    }

    // Check columns for a win
    for (int col = 0; col < SIZE; col++) {
        if (board[0][col] == board[1][col] && board[1][col] == board[2][col]) {
            if (board[0][col] == PLAYER_X) return PLAYER_X;  // Player X wins
            if (board[0][col] == PLAYER_O) return PLAYER_O;  // Player O wins
        }
    }

    // Check diagonal from top-left to bottom-right
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0] == PLAYER_X) return PLAYER_X;  // Player X wins
        if (board[0][0] == PLAYER_O) return PLAYER_O;  // Player O wins
    }

    // Check diagonal from top-right to bottom-left
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[0][2] == PLAYER_X) return PLAYER_X;  // Player X wins
        if (board[0][2] == PLAYER_O) return PLAYER_O;  // Player O wins
    }

    // No winner yet
    return -1;
}

// Function to evaluate the board state
int evaluate(int board[SIZE][SIZE]) {
    // Check rows
    for (int row = 0; row < SIZE; row++) {
        if (board[row][0] == board[row][1] && board[row][1] == board[row][2]) {
            if (board[row][0] == PLAYER_X) return +10;
            else if (board[row][0] == PLAYER_O) return -10;
        }
    }

    // Check columns
    for (int col = 0; col < SIZE; col++) {
        if (board[0][col] == board[1][col] && board[1][col] == board[2][col]) {
            if (board[0][col] == PLAYER_X) return +10;
            else if (board[0][col] == PLAYER_O) return -10;
        }
    }

    // Check diagonals
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0] == PLAYER_X) return +10;
        else if (board[0][0] == PLAYER_O) return -10;
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[0][2] == PLAYER_X) return +10;
        else if (board[0][2] == PLAYER_O) return -10;
    }

    return 0;
}

// Minimax function with alpha-beta pruning
int minimax(int board[SIZE][SIZE], int depth, int isMaximizingPlayer, int alpha, int beta) {
    int score = evaluate(board);

    // If X wins
    if (score == 10) return score;

    // If O wins
    if (score == -10) return score;

    // If no moves left, it's a draw
    if (isMovesLeft(board) == 0) return 0;

    if (isMaximizingPlayer) {
        int best = -1000;

        // Try all possible moves for X
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] == EMPTY) {
                    board[i][j] = PLAYER_X;
                    best = (best > minimax(board, depth + 1, 0, alpha, beta)) ? best : minimax(board, depth + 1, 0, alpha, beta);
                    board[i][j] = EMPTY;
                    alpha = (alpha > best) ? alpha : best;
                    if (beta <= alpha) break;
                }
            }
        }
        return best;
    } else {
        int best = 1000;

        // Try all possible moves for O
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] == EMPTY) {
                    board[i][j] = PLAYER_O;
                    best = (best < minimax(board, depth + 1, 1, alpha, beta)) ? best : minimax(board, depth + 1, 1, alpha, beta);
                    board[i][j] = EMPTY;
                    beta = (beta < best) ? beta : best;
                    if (beta <= alpha) break;
                }
            }
        }
        return best;
    }
}

// Function to check if there are moves left
int isMovesLeft(int board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == EMPTY) return 1;
        }
    }
    return 0;
}

// Function to make an AI move (with randomness for imperfection)
void aiMove(int board[SIZE][SIZE], int player) {
    int bestVal = (player == PLAYER_X) ? -1000 : 1000;
    int bestMove[2] = {-1, -1};
    
    // First, try to make the best move (minimax)
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == EMPTY) {
                board[i][j] = player;
                int moveVal = minimax(board, 0, player == PLAYER_O, -1000, 1000);
                board[i][j] = EMPTY;

                if ((player == PLAYER_X && moveVal > bestVal) || (player == PLAYER_O && moveVal < bestVal)) {
                    bestVal = moveVal;
                    bestMove[0] = i;
                    bestMove[1] = j;
                }
            }
        }
    }

    // Add randomness here (imperfect AI)
    if (rand() % 10 < 3) {  // 30% chance to make a suboptimal move
        randomMove(board);
        return;
    }

    // Make the best move
    board[bestMove[0]][bestMove[1]] = player;
}

// Function to make a random move for the AI
int randomMove(int board[SIZE][SIZE]) {
    int moves[SIZE * SIZE][2];
    int count = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == EMPTY) {
                moves[count][0] = i;
                moves[count][1] = j;
                count++;
            }
        }
    }
    if (count > 0) {
        int randIndex = rand() % count;
        board[moves[randIndex][0]][moves[randIndex][1]] = PLAYER_O;
    }
    return 0;
}

int main() {
    srand(time(0)); // Seed random number generator

    int board[SIZE][SIZE] = { {EMPTY, EMPTY, EMPTY},
                               {EMPTY, EMPTY, EMPTY},
                               {EMPTY, EMPTY, EMPTY} };
    
    int turn = 1; // 1 for PLAYER_X, 0 for PLAYER_O
    int winner = -1;  // -1: no winner, 1: PLAYER_X, 0: PLAYER_O

    while (isMovesLeft(board) && winner == -1) {
        printBoard(board);
        
        if (turn == 1) {  // Player's turn (X)
            int row, col;
            printf("Player X, enter your move (row and column): ");
            scanf("%d %d", &row, &col);
            if (board[row][col] == EMPTY) {
                board[row][col] = PLAYER_X;
                winner = checkWin(board);
                turn = 0;  // Switch to AI
            } else {
                printf("Cell already occupied, try again!\n");
            }
        } else {  // AI's turn (O)
            aiMove(board, PLAYER_O);
            winner = checkWin(board);
            turn = 1;  // Switch to Player
        }
    }

    printBoard(board);
    
    if (winner == PLAYER_X) printf("Player X wins!\n");
    else if (winner == PLAYER_O) printf("AI (Player O) wins!\n");
    else printf("It's a draw!\n");

    return 0;
}