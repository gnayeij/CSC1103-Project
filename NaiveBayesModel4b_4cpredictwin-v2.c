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

// Global win counter
int ai_win_counter = 0;

// Load Tic Tac Toe Dataset
int nb_load_dataset(const char *filename)
{
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

    // Increment win counter if the prediction is a win
    if (win_prob > lose_prob) 
    {
        ai_win_counter++;
        return 1; // Win
    } 
    else 
    {
        return 0; // Lose
    }
}

// Calculate the accuracy for the given dataset
double calculate_accuracy(MoveData *data_set, int size, int is_training) 
{
    int correct_predictions = 0;

    for (int i = 0; i < size; i++) 
    {
        int predicted_outcome = nb_predict(data_set[i].board);
        if (predicted_outcome == data_set[i].outcome) 
        {
            correct_predictions++;
        }
    }

    return ((double)correct_predictions / size) * 100.0;
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

                double win_prob = nb_model.prob_win;
                if (nb_predict(board) == 1) 
                { 
                    win_prob *= nb_model.prob_win; 
                } 
                else 
                {
                    win_prob *= nb_model.prob_lose;
                }

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

    int train_size = (int)(TICTACTOE_DATASET * TRAIN_TEST);
    double train_accuracy = calculate_accuracy(train_set, train_size, 1);

    int test_size = TICTACTOE_DATASET - train_size;
    double test_accuracy = calculate_accuracy(test_set, test_size, 0);

    printf("Training Accuracy: %.2f%%\n", train_accuracy);
    printf("Test Accuracy: %.2f%%\n", test_accuracy);
    printf("AI Win Counter: %d\n", ai_win_counter);

    // Example for AI move
    char test_board[BOARD_SIZE][BOARD_SIZE] = 
    {
        {'X', 'O', 'X'},
        {'O', 'X', 'O'},
        {' ', ' ', ' '}
    };
    
    printf("Board before AI move:\n");
    for (int i = 0; i < BOARD_SIZE; i++) 
    {
        for (int j = 0; j < BOARD_SIZE; j++) 
        {
            printf("%c ", test_board[i][j]);
        }
        printf("\n");
    }

    nb_ai_move(test_board);

    printf("Board after AI move:\n");
    for (int i = 0; i < BOARD_SIZE; i++) 
    {
        for (int j = 0; j < BOARD_SIZE; j++) 
        {
            printf("%c ", test_board[i][j]);
        }
        printf("\n");
    }

    return 0;
}
