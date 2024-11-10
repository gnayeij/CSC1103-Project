#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOARD_SIZE 3
#define EMPTY 2

// Structure to store a legal move and its resulting board state
typedef struct {
    int move_x;
    int move_y;
    int resulting_board[BOARD_SIZE * BOARD_SIZE];
} LegalMove;

// Structure to store all legal moves
typedef struct {
    LegalMove* moves;
    int count;
} LegalMoves;

// Function to create a copy of the board
void copy_board(int source[BOARD_SIZE][BOARD_SIZE], int dest[BOARD_SIZE][BOARD_SIZE]) {
    for(int i = 0; i < BOARD_SIZE; i++) {
        for(int j = 0; j < BOARD_SIZE; j++) {
            dest[i][j] = source[i][j];
        }
    }
}

// Function to flatten 2D board to 1D array
void flatten_board(int board[BOARD_SIZE][BOARD_SIZE], int flattened[BOARD_SIZE * BOARD_SIZE]) {
    int k = 0;
    for(int i = 0; i < BOARD_SIZE; i++) {
        for(int j = 0; j < BOARD_SIZE; j++) {
            flattened[k++] = board[i][j];
        }
    }
}

// Main function to generate legal moves
LegalMoves* legal_moves_generator(int current_board_state[BOARD_SIZE][BOARD_SIZE], int turn_monitor) {
    LegalMoves* legal_moves = malloc(sizeof(LegalMoves));
    legal_moves->moves = malloc(sizeof(LegalMove) * BOARD_SIZE * BOARD_SIZE); // Maximum possible moves
    legal_moves->count = 0;
    
    // Temporary board for creating copies
    int board_copy[BOARD_SIZE][BOARD_SIZE];
    
    // Check each position on the board
    for(int i = 0; i < BOARD_SIZE; i++) {
        for(int j = 0; j < BOARD_SIZE; j++) {
            // If the position is empty (represented by 2)
            if(current_board_state[i][j] == EMPTY) {
                // Create a copy of the current board
                copy_board(current_board_state, board_copy);
                
                // Make the move
                board_copy[i][j] = turn_monitor;
                
                // Create new legal move
                LegalMove* move = &legal_moves->moves[legal_moves->count];
                move->move_x = i;
                move->move_y = j;
                
                // Flatten the resulting board state
                flatten_board(board_copy, move->resulting_board);
                
                legal_moves->count++;
            }
        }
    }
    
    return legal_moves;
}

// Function to print a legal move (for debugging)
void print_legal_move(LegalMove* move) {
    printf("Move: (%d, %d)\n", move->move_x, move->move_y);
    printf("Resulting board state:\n");
    for(int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
        printf("%d ", move->resulting_board[i]);
        if((i + 1) % BOARD_SIZE == 0) printf("\n");
    }
    printf("\n");
}

// Function to print the current board state
void print_board(int board[BOARD_SIZE][BOARD_SIZE]) {
    printf("Current board state:\n");
    for(int i = 0; i < BOARD_SIZE; i++) {
        for(int j = 0; j < BOARD_SIZE; j++) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Function to free the legal moves structure
void free_legal_moves(LegalMoves* legal_moves) {
    free(legal_moves->moves);
    free(legal_moves);
}

int main() {
    // Create a sample board state
    int board[BOARD_SIZE][BOARD_SIZE] = {
        {2, 2, 2},
        {2, 1, 2},
        {2, 2, 2}
    };
    
    // Print initial board state
    printf("Initial board state:\n");
    print_board(board);
    
    // Generate legal moves for player 0
    LegalMoves* legal_moves = legal_moves_generator(board, 0);
    
    // Print all legal moves
    printf("Found %d legal moves:\n", legal_moves->count);
    for(int i = 0; i < legal_moves->count; i++) {
        print_legal_move(&legal_moves->moves[i]);
    }
    
    // Free memory
    free_legal_moves(legal_moves);
    
    return 0;
}