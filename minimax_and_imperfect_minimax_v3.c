#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> 

#define SIZE 3          // Size of Tic Tac Toe board
#define EMPTY -1        // Empty cell
#define PLAYER 1        // Player (X)
#define COMPUTER 0      // Computer (AI) (O)

// Function prototypes
void printBoard(int board[SIZE][SIZE]);
int checkWin(int board[SIZE][SIZE]);
int minimax(int board[SIZE][SIZE], int isMaximizingPlayer, int alpha, int beta);
int movesLeft(int board[SIZE][SIZE]);
int evaluateScore(int board[SIZE][SIZE]);
void aiMove(int board[SIZE][SIZE], int player, int difficulty);
int ranMove(int board[SIZE][SIZE]);
int getValidNumber();
void getValidMove(int board[SIZE][SIZE], int* row, int* col);

// Function to print the Tic-Tac-Toe board
void printBoard(int board[SIZE][SIZE]) {
    // printing the cells
    for (int i = 0; i < SIZE; i++) {        
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == PLAYER)
                // Print 'X' for player and 'O' for computer
                printf(" X ");
            else if (board[i][j] == COMPUTER)
                printf(" O ");
            else
                printf(" - ");

            // Print a vertical 'line' between columns that seperates the cell (except last column)
            if (j < SIZE - 1) 
                printf("|");
        }
        printf("\n");

        // Print a horizontal 'line' between rows to seperate the cells (except last row)
        if (i < SIZE - 1) 
            printf("-----------\n");
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
// This function assist the Minimax Algorithm as it provides the evaluation of the board position
// When it is Computer (AI) turn, the algorithm will minimize the score to increase the chance of AI winning (returning -10)
// When it is Player turn, the algorithm will maximize the score where the player is likely to win (returning 10)
int evaluateScore(int board[SIZE][SIZE]) {
    // Check rows for winning condition
    for (int row = 0; row < SIZE; row++) {
        if (board[row][0] == board[row][1] && board[row][1] == board[row][2]) {
            if (board[row][0] == PLAYER) return +10;            // Player wins
            else if (board[row][0] == COMPUTER) return -10;     // Computer wins
        }
    }

    // Check columns for winning condition
    for (int col = 0; col < SIZE; col++) {
        if (board[0][col] == board[1][col] && board[1][col] == board[2][col]) {
            if (board[0][col] == PLAYER) return +10;            // Player wins
            else if (board[0][col] == COMPUTER) return -10;     // Computer wins
        }
    }

    // Check diagonals for winning condition
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0] == PLAYER) return +10;                  // Player wins
        else if (board[0][0] == COMPUTER) return -10;           // Computer wins
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[0][2] == PLAYER) return +10;                  // Player wins
        else if (board[0][2] == COMPUTER) return -10;           // Computer wins
    }

    // If there is no winnder, then return the value of 0 which means its a draw
    return 0;
}

// Minimax function with alpha-beta pruning
int minimax(int board[SIZE][SIZE], int isMaximizingPlayer, int alpha, int beta) {
    int score = evaluateScore(board);       // Evaluate current state of board

    // If X wins
    if (score == 10) return score;

    // If O wins
    if (score == -10) return score;

    // If no moves left, it's a draw
    if (movesLeft(board) == 0) return 0;

    if (isMaximizingPlayer) {
        int best = -1000;       // Best score for maximizing player

        // Try all possible moves for X (Player) -- maximizing score
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] == EMPTY)       // If cell is empty
                {
                    board[i][j] = PLAYER;       // player move

                    // Call minimax recursively for the minimizing (Computer)
                    best = (best > minimax(board, 0, alpha, beta)) ? best : minimax(board, 0, alpha, beta);
                    
                    // Undo all moves 
                    board[i][j] = EMPTY;

                    // alpha-beta pruning
                    alpha = (alpha > best) ? alpha : best;
                    if (beta <= alpha) break;       // Prune the branch
                }
            }
        }
        return best;    // return best score for maximizng player
    } else {
        int best = 1000;        // best score for minimizing player (AI)

        // Try all possible moves for Computer
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] == EMPTY)       // if cell empty
                {
                    board[i][j] = COMPUTER;     // Computer turn

                    // Call minimax recursively for maximizing player (AI)
                    best = (best < minimax(board, 1, alpha, beta)) ? best : minimax(board, 1, alpha, beta);
                    
                    // Undo all moves
                    board[i][j] = EMPTY;

                    // alpha-beta pruning
                    beta = (beta < best) ? beta : best;
                    if (beta <= alpha) break;       // Prune the branch
                }
            }
        }
        return best;        // return best score for the minizming player
    }
}

// Function to check if there are moves left
int movesLeft(int board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == EMPTY) return 1;     // if there are moves left
        }
    }
    return 0;       // No moves left
}

// Function to make an AI move (with difficulty)
void aiMove(int board[SIZE][SIZE], int player, int difficulty) {
    int bestVal = (player == PLAYER) ? -1000 : 1000;
    int bestMove[2] = {-1, -1};

    // If the difficulty is Easy, AI makes a random move
    if (difficulty == 1) {
        ranMove(board);     // AI make random move
        return;
    }

    // For Medium or Hard difficulty, use minimax with or without randomness
    if (difficulty == 2 || difficulty == 3) {
        // Loop through all empty cells to find best move
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] == EMPTY)       // cell empty
                {
                    board[i][j] = player;       // make move for player
                    
                    // get score for the move using minimax algorithm
                    int moveVal = minimax(board, player == COMPUTER, -1000, 1000);
                    board[i][j] = EMPTY;    // undo move

                    // check if move is better than current move
                    if ((player == PLAYER && moveVal > bestVal) || (player == COMPUTER && moveVal < bestVal)) {
                        bestVal = moveVal;
                        bestMove[0] = i;    // store row of best move
                        bestMove[1] = j;    // store column of best move
                    }
                }
            }
        }
    }

    // Add randomness for medium difficulty (not always optimal)
    if (difficulty == 2 && rand() % 10 < 3) {
        ranMove(board);     // make random move for medium difficulty
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
        int randIndex = rand() % count;     // random select a valid move
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
            // Clear the input buffer  if invaid input
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