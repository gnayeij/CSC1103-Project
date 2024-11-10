#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TICTACTOE_BOARD 9
#define TICTACTOE_DATASET 958
#define TRAIN_TEST 0.8


// Define the structure of dataset
typedef struct 
{
    char board[TICTACTOE_BOARD]; // Player 1 is 'X', Player 2 is 'O', Blank cell is 'b'
    int outcome;            // Win is '1', Lose is '0'
} 
MoveData;

// Define the probabilities for Naive Bayes
typedef struct 
{
    double prob_x[TICTACTOE_BOARD][2]; // 'X' probabilities in each cell
    double prob_o[TICTACTOE_BOARD][2]; // 'O' probabilities in each cell
    double prob_blank[TICTACTOE_BOARD][2]; // 'b' probabilities in each cell
    double prob_win; // Prior probability of winning
    double prob_lose; // Prior probability of losing
} 
NaiveBayesModel;

// Global data arrays
MoveData dataset[TICTACTOE_DATASET];
MoveData train_set[(int)(TICTACTOE_DATASET * TRAIN_TEST)];
MoveData test_set[TICTACTOE_DATASET - (int)(TICTACTOE_DATASET * TRAIN_TEST)];
NaiveBayesModel model;

// AI win counter
int ai_win_count = 0;

// Load the dataset from file
int load_dataset(const char *filename) 
{
    FILE *file = fopen(filename, "r");
    if (!file) 
    {
        perror("Error! Tic Tac Toe Dataset unable to be loaded.");
        return -1;
    }

    for (int i = 0; i < TICTACTOE_DATASET; i++) 
    {
        fscanf(file, "%s %d", dataset[i].board, &dataset[i].outcome);
    }

    fclose(file);
    return 0;
}

// Split the dataset into train and test sets
void split_dataset() 
{
    int train_size = (int)(TICTACTOE_DATASET * TRAIN_TEST);
    for (int i = 0; i < train_size; i++) 
    {
        train_set[i] = dataset[i];
    }

    for (int i = train_size; i < TICTACTOE_DATASET; i++) 
    {
        test_set[i - train_size] = dataset[i];
    }
}

// Train the Naive Bayes Model
void train_nb() 
{
    int win_count = 0, lose_count = 0;
    int train_size = (int)(TICTACTOE_DATASET * TRAIN_TEST);

    for (int i = 0; i < train_size; i++) 
    {
        if (train_set[i].outcome == 1) win_count++;
        else lose_count++;

        for (int j = 0; j < TICTACTOE_BOARD; j++) 
        {
            if (train_set[i].outcome == 1) 
            {
                if (train_set[i].board[j] == 'x') model.prob_x[j][1]++;
                if (train_set[i].board[j] == 'o') model.prob_o[j][1]++;
                if (train_set[i].board[j] == 'b') model.prob_blank[j][1]++;
            } 

            else 
            {
                if (train_set[i].board[j] == 'x') model.prob_x[j][0]++;
                if (train_set[i].board[j] == 'o') model.prob_o[j][0]++;
                if (train_set[i].board[j] == 'b') model.prob_blank[j][0]++;
            }
        }
    }

    // Convert the counts to probabilities
    for (int j = 0; j < TICTACTOE_BOARD; j++) 
    {
        model.prob_x[j][1] /= win_count;
        model.prob_o[j][1] /= win_count;
        model.prob_blank[j][1] /= win_count;

        model.prob_x[j][0] /= lose_count;
        model.prob_o[j][0] /= lose_count;
        model.prob_blank[j][0] /= lose_count;
    }

    model.prob_win = (double)win_count / train_size;
    model.prob_lose = (double)lose_count / train_size;
}

// Predict the game outcome using the Naive Bayes Model
int predict(char *board) 
{
    double win_prob = model.prob_win;
    double lose_prob = model.prob_lose;

    for (int i = 0; i < TICTACTOE_BOARD; i++) 
    {
        if (board[i] == 'x') 
        {
            win_prob *= model.prob_x[i][1];
            lose_prob *= model.prob_x[i][0];
        } 

        else if (board[i] == 'o') 
        {
            win_prob *= model.prob_o[i][1];
            lose_prob *= model.prob_o[i][0];
        } 

        else 
        {
            win_prob *= model.prob_blank[i][1];
            lose_prob *= model.prob_blank[i][0];
        }
    }

    return win_prob > lose_prob ? 1 : 0;
}

// Tic Tac Toe Board
char board[TICTACTOE_BOARD] = {'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b'};

// Print the current state of Tic Tac Toe Board
void print_board() 
{
    printf("Current Board:\n");
    for (int i = 0; i < TICTACTOE_BOARD; i++) 
    {
        printf("%c ", board[i] == 'b' ? '_' : board[i]);
        if ((i + 1) % 3 == 0) printf("\n");
    }

    printf("\n");
}

// Check if a player has won
int check_winner(char player) 
{
    int win_positions[8][3] = 
    {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, // Rows
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, // Columns
        {0, 4, 8}, {2, 4, 6}             // Diagonals
    };

    for (int i = 0; i < 8; i++) 
    {
        if (board[win_positions[i][0]] == player &&
            board[win_positions[i][1]] == player &&
            board[win_positions[i][2]] == player) 
            {
            return 1;
            }
    }

    return 0;
}

// Handle moves made by player
void player_move() 
{
    int position;
    printf("Enter your move (1-9): ");
    while (1) 
    {
        scanf("%d", &position);
        if (position < 1 || position > 9 || board[position - 1] != 'b') 
        {
            printf("Invalid move. Please try again: ");
        } 
        
        else 
        {
            board[position - 1] = 'x';
            break;
        }
    }
}

// Handle moves made by AI
void ai_move() 
{
    int best_move = -1;
    for (int i = 0; i < TICTACTOE_BOARD; i++) 
    {
        if (board[i] == 'b') 
        { // Empty space
            board[i] = 'o';    // Tentatively make the move
            int prediction = predict(board);
            if (prediction == 1) 
            { // AI believes this is a winning move
                best_move = i;
                board[i] = 'b';    // Undo move
                break;
            }

            board[i] = 'b';        // Undo move
        }
    }
    
    if (best_move == -1) 
    { // No win can be predicted, first empty spot to be picked
        for (int i = 0; i < TICTACTOE_BOARD; i++) 
        {
           
            if (board[i] == 'b') 
            {
                best_move = i;
                break;
            }
        }
    }
    
    board[best_move] = 'o';
    printf("AI chooses position %d\n", best_move + 1);
}

// Main game loop
void play_game() 
{
    int turn = 0; // '0' for Player, '1' for AI
    while (1) 
    {
        print_board();
        if (turn == 0) 
        {
            player_move();
            if (check_winner('x')) 
            {
                print_board();
                printf("Player wins!\n");
                break;
            }
        } 
        
        else 
        {
            ai_move();
            if (check_winner('o')) 
            {
                print_board();
                printf("AI wins!\n");
                ai_win_count++; // AI win counter increment
                break;
            }
        }

        turn = 1 - turn; // Take alternate turns
        int empty = 0;
        for (int i = 0; i < TICTACTOE_BOARD; i++) 
        {
            if (board[i] == 'b') 
            {
                empty = 1;
                break;
            }
        }

        if (!empty) 
        {
            print_board();
            printf("It's a draw!\n");
            break;
        }
    }
}

int main() 
{
    // Load and split dataset, train Naive Bayes model
    if (load_dataset("tic_tac_toe_dataset.txt") != 0) 
    {
        return 1;
    }

    split_dataset();
    train_nb();

    // Play multiple games to gauge difficulty level
    char play_again;
    do 
    {
        memset(board, 'b', TICTACTOE_BOARD); // Reset board
        play_game();

        // Display current AI win count
        printf("Current win count of AI: %d\n", ai_win_count);
        printf("Play again? (y/n): ");
        scanf(" %c", &play_again);
    } 
    
    while (play_again == 'y');

    // Display final difficulty level based on AI wins
    printf("Total win count of AI: %d\n", ai_win_count);
    printf("Difficulty level based on number of games AI won: %s\n",
           ai_win_count > 5 ? "Hard" : ai_win_count > 2 ? "Medium" : "Easy");

    return 0;
}
