#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TICTACTOE_DATASET 958
#define TRAIN_TEST 0.8
#define TICTACTOE_BOARD 9

// Define the dataset structure
typedef struct 
{
    char board[TICTACTOE_BOARD]; // player 1 is 'X', player 2 is 'O', blank is 'b'
    int outcome;            // 1 for win, 0 for lose
} 
MoveData;

// Define probabilities for Naive Bayes
typedef struct 
{
    double prob_x[TICTACTOE_BOARD][3]; // 'X' probabilities in each cell
    double prob_o[TICTACTOE_BOARD][3]; // 'O' probabilities in each cell
    double prob_blank[TICTACTOE_BOARD][3]; // 'b' probabilities in each cell
    double prob_win; // Prior winning probability
    double prob_lose; // Prior losing probability
} 
NaiveBayesModel;

// Global data arrays
MoveData dataset[TICTACTOE_DATASET];
NaiveBayesModel model;

// Load dataset from file
int load_dataset(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) 
    {
        perror("Error opening file");
        return -1;
    }

    for (int i = 0; i < TICTACTOE_DATASET; i++) 
    {
        fscanf(file, "%s %d", dataset[i].board, &dataset[i].outcome);
    }
    fclose(file);
    return 0;
}

// Split dataset into train and test sets
int train_size = (int)(TICTACTOE_DATASET * TRAIN_TEST);
MoveData train_set[(int)(TICTACTOE_DATASET * TRAIN_TEST)];
MoveData test_set[TICTACTOE_DATASET - (int)(TICTACTOE_DATASET * TRAIN_TEST)];

void split_dataset() 
{
    for (int i = 0; i < train_size; i++) 
    {
        train_set[i] = dataset[i];
    }

    for (int i = train_size; i < TICTACTOE_DATASET; i++) 
    {
        test_set[i - train_size] = dataset[i];
    }
}

// Train Naive Bayes Model
void train_naive_bayes() 
{
    int win_count = 0, lose_count = 0;

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

    // Convert counts to probabilities
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

// Predict outcome using Naive Bayes Model
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

// Calculate accuracy and confusion matrix
void evaluate_model()
{
    int tp = 0, tn = 0, fp = 0, fn = 0;

    for (int i = 0; i < TICTACTOE_DATASET - train_size; i++) 
    {
        int actual = test_set[i].outcome;
        int predicted = predict(test_set[i].board);

        if (predicted == 1 && actual == 1) tp++;
        else if (predicted == 0 && actual == 0) tn++;
        else if (predicted == 1 && actual == 0) fp++;
        else if (predicted == 0 && actual == 1) fn++;
    }

    printf("Confusion Matrix:\n");
    printf("TP: %d, TN: %d, FP: %d, FN: %d\n", tp, tn, fp, fn);

    double accuracy = (double)(tp + tn) / (tp + tn + fp + fn);
    printf("Accuracy: %.2f%%\n", accuracy * 100);
}

int main() 
{
    // Load and split dataset
    if (load_dataset("tic_tac_toe_dataset.txt") != 0) 
    {
        return 1;
    }
    split_dataset();

    // Train Naive Bayes model
    train_naive_bayes();

    // Evaluate model performance
    evaluate_model();

    // Replace Minimax with predict() to make "imperfect" AI moves

    return 0;
}