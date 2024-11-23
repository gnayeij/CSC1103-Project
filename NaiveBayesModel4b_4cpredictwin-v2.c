// Naive Bayes Model
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TICTACTOE_DATASET 958
#define TRAIN_TEST 0.8
#define BOARD_SIZE 3
typedef struct 
{
    char board[BOARD_SIZE][BOARD_SIZE];
    int outcome;
} MoveData;

// Structure of NB Probabilities
typedef struct
{
    double prob_x[BOARD_SIZE][BOARD_SIZE][2];
    double prob_o[BOARD_SIZE][BOARD_SIZE][2];
    double prob_blank[BOARD_SIZE][BOARD_SIZE][2];
    double prob_win;
    double prob_lose;
} NavieBayesModel;

// Arrays to store Tic Tac Toe dataset and split it into training and test set
MoveData dataset[TICTACTOE_DATASET];
MoveData train_set[(int)(TICTACTOE_DATASET * TRAIN_TEST)];
MoveData test_set[TICTACTOE_DATASET - (int)(TICTACTOE_DATASET * TRAIN_TEST)];
NavieBayesModel nb_model;

int nb_load_dataset(const char *filename){
    FILE *file = fopen(filename, "r");
    if (!file) 
    {
        perror("Error! Tic Tac Toe Dataset unable to be loaded.");
        return -1;
    }
    for (int i = 0; i < TICTACTOE_DATASET; i++) 
    {
        for (int row = 0; row < BOARD_SIZE; row++) 
        {
            fscanf(file, "%s", dataset[i].board[row]);
        }
        fscanf(file, "%d", &dataset[i].outcome);
    }

    fclose(file);
    return 0;
}

// Split the dataset into train and test sets based on TRAIN_TEST
void split_dataset() 
{
    int train_size = (int)(TICTACTOE_DATASET * TRAIN_TEST);
    
    // Fill training set up to train_size
    for (int i = 0; i < train_size; i++) 
    {
        train_set[i] = dataset[i];
    }

    // Put remaining data into the test set
    for (int i = train_size; i < TICTACTOE_DATASET; i++) 
    {
        test_set[i - train_size] = dataset[i];
    }
}

// Train the Navie Bayes Model using training data
void train_nb() 
{
    int win_count = 0, lose_count = 0;
    int train_size = (int)(TICTACTOE_DATASET * TRAIN_TEST);

    // Loop through the training set, counting both wins and losses
    for (int i = 0; i < train_size; i++) 
    {
        if (train_set[i].outcome == 1) win_count++;
        else lose_count++;

        // Update each cell counts based on outcome of game
        for (int row = 0; row < BOARD_SIZE; row++) 
        {
            for (int col = 0; col < BOARD_SIZE; col++) 
            {
                if (train_set[i].outcome == 1) // Game outcome is win
                {
                    if (train_set[i].board[row][col] == 'X') nb_model.prob_x[row][col][1]++;
                    if (train_set[i].board[row][col] == 'O') nb_model.prob_o[row][col][1]++;
                    if (train_set[i].board[row][col] == ' ') nb_model.prob_blank[row][col][1]++;
                } 
                else // Game outcome is lose
                {
                    if (train_set[i].board[row][col] == 'X') nb_model.prob_x[row][col][0]++;
                    if (train_set[i].board[row][col] == 'O') nb_model.prob_o[row][col][0]++;
                    if (train_set[i].board[row][col] == ' ') nb_model.prob_blank[row][col][0]++;
                }
            }
        }
    }

    // Calculate the probability from counts
    for (int row = 0; row < BOARD_SIZE; row++) 
    {
        for (int col = 0; col < BOARD_SIZE; col++) 
        {
            nb_model.prob_x[row][col][1] /= win_count;
            nb_model.prob_o[row][col][1] /= win_count;
            nb_model.prob_blank[row][col][1] /= win_count;

            nb_model.prob_x[row][col][0] /= lose_count;
            nb_model.prob_o[row][col][0] /= lose_count;
            nb_model.prob_blank[row][col][0] /= lose_count;
        }
    }

    // Set overall win and lose probabilities
    nb_model.prob_win = (double)win_count / train_size;
    nb_model.prob_lose = (double)lose_count / train_size;
}

// Predict the game outcome using the Navie Bayes Model based on given Tic Tac Toe board state
int nb_predict(char board[BOARD_SIZE][BOARD_SIZE]) 
{
    double win_prob = nb_model.prob_win;
    double lose_prob = nb_model.prob_lose;

    for (int row = 0; row < BOARD_SIZE; row++) 
    {
        for (int col = 0; col < BOARD_SIZE; col++) 
        {
            if (board[row][col] == 'X') 
            {
                win_prob *= nb_model.prob_x[row][col][1];
                lose_prob *= nb_model.prob_x[row][col][0];
            } 
            else if (board[row][col] == 'O') 
            {
                win_prob *= nb_model.prob_o[row][col][1];
                lose_prob *= nb_model.prob_o[row][col][0];
            } 
            else 
            {
                win_prob *= nb_model.prob_blank[row][col][1];
                lose_prob *= nb_model.prob_blank[row][col][0];
            }
        }
    }

    return win_prob > lose_prob ? 1 : 0; // Return '1' for win, '0' for lose
}

void nb_ai_move(char board[BOARD_SIZE][BOARD_SIZE]) 
{
    int best_row = -1;
    int best_col = -1;
    double highest_prob = -1.0;

    // Check each possible move on the board to see which has the highest chance to win
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++) 
        {
            if (board[row][col] == ' ') // Empty cell
            { 
                board[row][col] = 'O'; // Make the move tentatively

                // Predict the outcome if this move is made on empty cell
                double win_prob = nb_model.prob_win; // Use Naive Bayes winning probability
                if (nb_predict(board) == 1) 
                { 
                    win_prob *= nb_model.prob_win; // AI's predicted move leads to a win
                } 

                else 
                {
                    win_prob *= nb_model.prob_lose;
                }

                // Track the move with the highest winning probability
                if (win_prob > highest_prob) 
                {
                    highest_prob = win_prob;
                    best_row = row;
                    best_col = col;
                }

                board[row][col] = ' '; // Undo the move after evaluation
            }
        }
    }


    // Make the best move found
    if (best_row != -1 && best_col != -1) 
    {
        board[best_row][best_col] = 'O';
    } 

}

int main() 
{
    if (nb_load_dataset("ml_algos/tic-tac-toe.txt") != 0) 
    {
        return 1;
    }

    split_dataset();
    train_nb();

    char test_board[BOARD_SIZE][BOARD_SIZE] = 
    {
        {'X', 'O', 'X'},
        {'O', 'X', 'O'},
        {' ', ' ', ' '}
    };
    printf(test_board);
    printf("\n");
    nb_ai_move(test_board);
    printf(test_board);
    //printf("Prediction for the given Tic Tac Toe board: %s\n", prediction == 1 ? "Win" : "Lose");

    return 0;
}