#include <stdio.h>

#define SIZE 3
#define EMPTY '.'  // Using '.' for empty spaces
#define X 'X'      // Player 1
#define O 'O'      // Player 2

// Simple structure for a move
typedef struct {
    int x, y;
} Move;

// Structure to hold all moves using static array
typedef struct {
    Move moves[SIZE * SIZE];
    int count;
} Moves;

// Print the game board
void print_board(char board[SIZE][SIZE]) {
    printf("\nBoard:\n");
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

// Generate all possible moves
Moves get_legal_moves(char board[SIZE][SIZE]) {
    Moves moves = {.count = 0};
    
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            if(board[i][j] == EMPTY) {
                moves.moves[moves.count].x = i;
                moves.moves[moves.count].y = j;
                moves.count++;
            }
        }
    }
    
    return moves;
}

// Print all possible moves
void print_moves(Moves moves) {
    printf("\nLegal moves (%d):\n", moves.count);
    for(int i = 0; i < moves.count; i++) {
        printf("Move %d: (%d, %d)\n", i + 1, moves.moves[i].x, moves.moves[i].y);
    }
}

int main() {
    // Sample board with one move made
    char board[SIZE][SIZE] = {
        {'.', '.', '.'},
        {'.', 'O', '.'},
        {'.', '.', '.'}
    };
    
    print_board(board);
    
    Moves moves = get_legal_moves(board);
    print_moves(moves);
    
    return 0;
}