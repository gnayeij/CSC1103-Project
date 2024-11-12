#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> 

#define SIZE 3
#define EMPTY -1
#define PLAYER 1
#define COMPUTER 0

// Function prototypes
void printBoard(int board[SIZE][SIZE]);
int checkWin(int board[SIZE][SIZE]);
int minimax(int board[SIZE][SIZE], int depth, int isMaximizingPlayer, int alpha, int beta);
int movesLeft(int board[SIZE][SIZE]);
int evaluateScore(int board[SIZE][SIZE]);
void aiMove(int board[SIZE][SIZE], int player, int difficulty);
int ranMove(int board[SIZE][SIZE]);
int getValidNumber();
void getValidMove(int board[SIZE][SIZE], int* row, int* col);

// Function to print the Tic-Tac-Toe board
void printBoard(int board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == PLAYER)
                printf(" X ");
            else if (board[i][j] == COMPUTER)
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
            if (board[row][0] == PLAYER) return PLAYER;  // Player X wins
            if (board[row][0] == COMPUTER) return COMPUTER;  // Player O wins
        }
    }

    // Check columns for a win
    for (int col = 0; col < SIZE; col++) {
        if (board[0][col] == board[1][col] && board[1][col] == board[2][col]) {
            if (board[0][col] == PLAYER) return PLAYER;  // Player X wins
            if (board[0][col] == COMPUTER) return COMPUTER;  // Player O wins
        }
    }

    // Check diagonal from top-left to bottom-right
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0] == PLAYER) return PLAYER;  // Player X wins
        if (board[0][0] == COMPUTER) return COMPUTER;  // Player O wins
    }

    // Check diagonal from top-right to bottom-left
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[0][2] == PLAYER) return PLAYER;  // Player X wins
        if (board[0][2] == COMPUTER) return COMPUTER;  // Player O wins
    }

    // No winner yet
    return -1;
}

// Function to evaluate the board state
int evaluateScore(int board[SIZE][SIZE]) {
    // Check rows
    for (int row = 0; row < SIZE; row++) {
        if (board[row][0] == board[row][1] && board[row][1] == board[row][2]) {
            if (board[row][0] == PLAYER) return +10;
            else if (board[row][0] == COMPUTER) return -10;
        }
    }

    // Check columns
    for (int col = 0; col < SIZE; col++) {
        if (board[0][col] == board[1][col] && board[1][col] == board[2][col]) {
            if (board[0][col] == PLAYER) return +10;
            else if (board[0][col] == COMPUTER) return -10;
        }
    }

    // Check diagonals
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0] == PLAYER) return +10;
        else if (board[0][0] == COMPUTER) return -10;
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[0][2] == PLAYER) return +10;
        else if (board[0][2] == COMPUTER) return -10;
    }

    return 0;
}

// Minimax function with alpha-beta pruning
int minimax(int board[SIZE][SIZE], int depth, int isMaximizingPlayer, int alpha, int beta) {
    int score = evaluateScore(board);

    // If X wins
    if (score == 10) return score;

    // If O wins
    if (score == -10) return score;

    // If no moves left, it's a draw
    if (movesLeft(board) == 0) return 0;

    if (isMaximizingPlayer) {
        int best = -1000;

        // Try all possible moves for X
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] == EMPTY) {
                    board[i][j] = PLAYER;
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
                    board[i][j] = COMPUTER;
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
int movesLeft(int board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == EMPTY) return 1;
        }
    }
    return 0;
}

// Function to make an AI move (with difficulty)
void aiMove(int board[SIZE][SIZE], int player, int difficulty) {
    int bestVal = (player == PLAYER) ? -1000 : 1000;
    int bestMove[2] = {-1, -1};

    // If the difficulty is Easy, AI makes a random move
    if (difficulty == 1) {
        ranMove(board);
        return;
    }

    // For Medium or Hard, use minimax with or without randomness
    if (difficulty == 2 || difficulty == 3) {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] == EMPTY) {
                    board[i][j] = player;
                    int moveVal = minimax(board, 0, player == COMPUTER, -1000, 1000);
                    board[i][j] = EMPTY;

                    if ((player == PLAYER && moveVal > bestVal) || (player == COMPUTER && moveVal < bestVal)) {
                        bestVal = moveVal;
                        bestMove[0] = i;
                        bestMove[1] = j;
                    }
                }
            }
        }
    }

    // Add randomness for medium difficulty (not always optimal)
    if (difficulty == 2 && rand() % 10 < 3) {
        ranMove(board);
        return;
    }

    // Make the best move
    board[bestMove[0]][bestMove[1]] = player;
}

// Function to make a random move for the AI
int ranMove(int board[SIZE][SIZE]) {
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
        board[moves[randIndex][0]][moves[randIndex][1]] = COMPUTER;
    }
    return 0;
}

// Function to get a valid number (for difficulty selection)
int getValidNumber() {
    int num;
    while (1) {
        printf("Select AI difficulty (1 = Easy, 2 = Medium, 3 = Hard): ");
        if (scanf("%d", &num) == 1 && num >= 1 && num <= 3) {
            return num;
        } else {
            // Clear the input buffer
            while (getchar() != '\n');
            printf("Invalid input! Please enter a number between 1 and 3.\n");
        }
    }
}

// Function to get a valid move from the user
void getValidMove(int board[SIZE][SIZE], int* row, int* col) {
    char input[100];  // Buffer for user input
    while (1) {
        printf("Enter your move (row and column): ");
        
        // Read the entire line of input as a string
        if (fgets(input, sizeof(input), stdin) == NULL) {
            // If input fails, prompt again
            while (getchar() != '\n');  // Clear the input buffer
            printf("Error reading input. Please try again.\n");
            continue;
        }

        // Remove trailing newline character if present
        input[strcspn(input, "\n")] = '\0';

        // Check if input has exactly two numbers separated by a space
        int count = 0;
        char *token = strtok(input, " ");  // Tokenize the string based on space
        int numbers[2] = {0, 0};
        
        // Try to extract two tokens (row and column)
        while (token != NULL && count < 2) {
            if (sscanf(token, "%d", &numbers[count]) == 1) {
                count++;
            } else {
                break;  // If the token is not an integer, stop parsing
            }
            token = strtok(NULL, " ");  // Get the next token
        }

        // Check if we have exactly two valid integers
        if (count == 2) {
            *row = numbers[0];
            *col = numbers[1];

            // Check if the row and column are within valid bounds
            if (*row >= 0 && *row < SIZE && *col >= 0 && *col < SIZE) {
                // Check if the cell is empty
                if (board[*row][*col] == EMPTY) {
                    break;  // Valid move, exit the loop
                } else {
                    printf("Cell already occupied, try again!\n");
                }
            } else {
                printf("Invalid input! Row and column must be between 0 and 2.\n");
            }
        } else {
            printf("Invalid input! Please enter two integers (row and column) separated by a space.\n");
        }
    }
}

int main() {
    srand(time(0)); // Seed random number generator

    // Ask user for difficulty level
    int difficulty = getValidNumber();

    // Clear the input buffer to avoid leftover characters affecting later inputs
    while (getchar() != '\n');  // Clear any leftover newline characters

    int board[SIZE][SIZE] = { {EMPTY, EMPTY, EMPTY},
                               {EMPTY, EMPTY, EMPTY},
                               {EMPTY, EMPTY, EMPTY} };

    int turn = 1; // 1 for PLAYER_X, 0 for PLAYER_O
    int winner = -1;  // -1: no winner, 1: PLAYER_X, 0: PLAYER_O

    while (movesLeft(board) && winner == -1) {
        printBoard(board);
        
        if (turn == 1) {  // Player's turn (X)
            int row, col;
            getValidMove(board, &row, &col);  // Get valid move from player
            board[row][col] = PLAYER;
            winner = checkWin(board);
            turn = 0;  // Switch to AI
        } else {  // AI's turn (O)
            aiMove(board, COMPUTER, difficulty);
            winner = checkWin(board);
            turn = 1;  // Switch to Player
        }
    }

    printBoard(board);
    
    if (winner == PLAYER) printf("Player X wins!\n");
    else if (winner == COMPUTER) printf("AI (Player O) wins!\n");
    else printf("It's a draw!\n");

    return 0;
}