#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TICTACTOE_ROWS 3
#define TICTACTOE_COLS 3
#define TICTACTOE_DATASET 958
#define TRAIN_TEST 0.8

// Structure of Tic Tac Toe Game state and result
typedef struct 
{
    char board[TICTACTOE_ROWS][TICTACTOE_COLS]; // 3x3 Tic Tac Toe board: Player 1 symbol is 'X', Player 2 symbol is 'O', or blank cell is 'b'
    int outcome; // Win is '1', Lose is '0'
} 
MoveData;

// Structure of Naive Bayes probabilities
typedef struct 
{
    double prob_x[TICTACTOE_ROWS][TICTACTOE_COLS][2]; // 'X' probabilities in each cell
    double prob_o[TICTACTOE_ROWS][TICTACTOE_COLS][2]; // 'O' probabilities in each cell
    double prob_blank[TICTACTOE_ROWS][TICTACTOE_COLS][2]; // 'b' probabilities in each cell
    double prob_win; // Prior probability of winning
    double prob_lose; // Prior probability of losing
} 
NaiveBayesModel;

// Arrays to store Tic Tac Toe dataset and split it into training and test set
MoveData dataset[TICTACTOE_DATASET];
MoveData train_set[(int)(TICTACTOE_DATASET * TRAIN_TEST)];
MoveData test_set[TICTACTOE_DATASET - (int)(TICTACTOE_DATASET * TRAIN_TEST)];
NaiveBayesModel model;

// Tic Tac Toe Board
char board[TICTACTOE_ROWS][TICTACTOE_COLS] = 
{
    {'b', 'b', 'b'},
    {'b', 'b', 'b'},
    {'b', 'b', 'b'}
};

// Load the dataset from file and populate moves array
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
        for (int row = 0; row < TICTACTOE_ROWS; row++) 
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

// Train the Naive Bayes Model using training data
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
        for (int row = 0; row < TICTACTOE_ROWS; row++) 
        {
            for (int col = 0; col < TICTACTOE_COLS; col++) 
            {
                if (train_set[i].outcome == 1) // Game outcome is win
                {
                    if (train_set[i].board[row][col] == 'x') model.prob_x[row][col][1]++;
                    if (train_set[i].board[row][col] == 'o') model.prob_o[row][col][1]++;
                    if (train_set[i].board[row][col] == 'b') model.prob_blank[row][col][1]++;
                } 
                else // Game outcome is lose
                {
                    if (train_set[i].board[row][col] == 'x') model.prob_x[row][col][0]++;
                    if (train_set[i].board[row][col] == 'o') model.prob_o[row][col][0]++;
                    if (train_set[i].board[row][col] == 'b') model.prob_blank[row][col][0]++;
                }
            }
        }
    }

    // Calculate the probability from counts
    for (int row = 0; row < TICTACTOE_ROWS; row++) 
    {
        for (int col = 0; col < TICTACTOE_COLS; col++) 
        {
            model.prob_x[row][col][1] /= win_count;
            model.prob_o[row][col][1] /= win_count;
            model.prob_blank[row][col][1] /= win_count;

            model.prob_x[row][col][0] /= lose_count;
            model.prob_o[row][col][0] /= lose_count;
            model.prob_blank[row][col][0] /= lose_count;
        }
    }

    // Set overall win and lose probabilities
    model.prob_win = (double)win_count / train_size;
    model.prob_lose = (double)lose_count / train_size;
}

// Predict the game outcome using the Naive Bayes Model based on given Tic Tac Toe board state
int predict(char board[TICTACTOE_ROWS][TICTACTOE_COLS]) 
{
    double win_prob = model.prob_win;
    double lose_prob = model.prob_lose;

    for (int row = 0; row < TICTACTOE_ROWS; row++) 
    {
        for (int col = 0; col < TICTACTOE_COLS; col++) 
        {
            if (board[row][col] == 'x') 
            {
                win_prob *= model.prob_x[row][col][1];
                lose_prob *= model.prob_x[row][col][0];
            } 
            else if (board[row][col] == 'o') 
            {
                win_prob *= model.prob_o[row][col][1];
                lose_prob *= model.prob_o[row][col][0];
            } 
            else 
            {
                win_prob *= model.prob_blank[row][col][1];
                lose_prob *= model.prob_blank[row][col][0];
            }
        }
    }

    return win_prob > lose_prob ? 1 : 0; // Return '1' for win, '0' for lose
}

// Display the current state of Tic Tac Toe Board
void print_board() 
{
    printf("Current Board:\n");
    for (int row = 0; row < TICTACTOE_ROWS; row++) 
    {
        for (int col = 0; col < TICTACTOE_COLS; col++) 
        {
            printf("%c ", board[row][col] == 'b' ? '_' : board[row][col]);
        }
        printf("\n");
    }
    printf("\n");
}

// Main function
int main() 
{
    if (load_dataset("tic_tac_toe_dataset.txt") != 0) 
    {
        return 1;
    }

    split_dataset();
    train_nb();

    char test_board[TICTACTOE_ROWS][TICTACTOE_COLS] = 
    {
        {'x', 'o', 'x'},
        {'o', 'x', 'o'},
        {'b', 'b', 'b'}
    };

    int prediction = predict(test_board);

    printf("Prediction for the given Tic Tac Toe board: %s\n", prediction == 1 ? "Win" : "Lose");

    return 0;
}
