#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "dataset_loader.h"
#include "move_data.h"
#include "linearRegressionModel.h"

#define DATASET_SIZE 958
#define BOARD_TOTAL_SIZE 9

double eval_approximation(int features[7], double weights[7])
{
    double score = 0.0;
    for (int i = 0; i < 7; i++)
    {
        score += features[i] * weights[i];
    }
    return score;
}

void board_to_features(char board[SIZE][SIZE], int features[7], char type)
{
    /*
    x0 = 1 constant
    x1 = 1 Number of rows/columns/diagonals with two of our own pieces and one empty field
    x2 = 1 Number of rows/columns/diagnonals with two of opponent's pieces and one empty field
    x3 = 1 Our own piece in centre?
    x4 = 1 Number of own pieces in corners
    x5 = 1 Number of rows/columns/diagonals with one own piece and two empty fields
    x6 = 1 Number of rows/columns/diagnoals with three own pieces
    */

    // determine opponent shape
    char opponent_type = 'O';
    if (type == 'O')
    {
        opponent_type = 'X';
    }

    if (board[1][1] == type)
    {
        features[3]++;
    }
    if (board[0][0] == type)
    {
        features[4]++;
    }
    if (board[2][2] == type)
    {
        features[4]++;
    }
    if (board[0][2] == type)
    {
        features[4]++;
    }
    if (board[2][0] == type)
    {
        features[4]++;
    }

    for (int i = 0; i < 3; i++)
    {
        int own_rows = 0, own_cols = 0, en_rows = 0, en_cols = 0, emp_rows = 0, emp_cols = 0;
        for (int j = 0; j < 3; j++)
        {
            if (board[i][j] == opponent_type)
            {
                en_rows++;
            }
            else if (board[i][j] == type)
            {
                own_rows++;
            }
            else
            {
                emp_rows++;
            }
            if (board[j][i] == opponent_type)
            {
                en_cols++;
            }
            else if (board[j][i] == type)
            {
                own_cols++;
            }
            else
            {
                emp_cols++;
            }
        }
        if (own_rows == 2 && emp_rows == 1)
        {
            features[1]++;
        }
        if (en_rows == 2 && emp_rows == 1)
        {
            features[2]++;
        }
        if (own_cols == 2 && emp_cols == 1)
        {
            features[1]++;
        }
        if (en_cols == 2 && emp_cols == 1)
        {
            features[2]++;
        }

        if (own_rows == 1 && emp_rows == 2)
        {
            features[5]++;
        }
        if (own_cols == 1 && emp_cols == 2)
        {
            features[5]++;
        }
        if (own_rows == 3)
        {
            features[6]++;
        }
        if (own_cols == 3)
        {
            features[6]++;
        }
    }

    for (int i = 0; i < 2; i++)
    {
        // diag check
        int own_diag = 0, en_diag = 0, emp_diag = 0;
        char diagonal;
        for (int j = 0; j < SIZE; j++)
        {
            if (i == 0)
            {
                diagonal = board[2 - j][j];
            }
            else
            {
                diagonal = board[j][j];
            }
            if (diagonal == type)
            {
                own_diag++;
            }
            else if (diagonal == opponent_type)
            {
                en_diag++;
            }
            else
            {
                emp_diag++;
            }
        }
        if (own_diag == 2 && emp_diag == 1)
        {
            features[1]++;
        }
        if (en_diag == 2 && emp_diag == 1)
        {
            features[2]++;
        }
        if (own_diag == 1 && emp_diag == 2)
        {
            features[5]++;
        }
        if (own_diag == 3)
        {
            features[6]++;
        }
    }
}

void initialize_weights(double weights[7])
{
    for (int i = 0; i < 7; i++)
    {
        weights[i] = ((double)rand() / RAND_MAX) * 0.2 - 0.1;
    }
}

int predict(char board[SIZE][SIZE], LinearRegressionModel model, char type)
{
    int counter = 0;
    char(*boards)[SIZE][SIZE] = malloc(BOARD_TOTAL_SIZE * sizeof(char[SIZE][SIZE]));
    int *moves = malloc(BOARD_TOTAL_SIZE * sizeof(int));
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (board[i][j] == 'B' || board[i][j] == ' ')
            {

                memcpy(boards[counter], board, sizeof(char[SIZE][SIZE]));
                boards[counter][i][j] = type;
                moves[counter] = i * 3 + j;
                counter++;
            }
        }
    }
    double val = -99999999;
    int best_move = -1;
    for (int i = 0; i < counter; i++)
    {
        int features[7] = {1, 0, 0, 0, 0, 0, 0};
        board_to_features(boards[i], features, type);
        double prediction = eval_approximation(features, model.weights);
        if (val < prediction)
        {
            val = prediction;
            best_move = moves[i];
        }
    }
    return best_move;
}

void train(LinearRegressionModel *model, int verbose)
{
    printf("Starting training with following parameters: Learning Rate: %.4f, Epochs: %d...\n", model->learning_rate, model->epochs);
    int num_samples = DATASET_SIZE * 0.8;

    for (int epoch = 0; epoch < model->epochs; epoch++)
    {
        double total_loss = 0.0;
        // shuffle dataset
        shuffle_data(model->dataset, DATASET_SIZE);
        for (int i = 0; i < num_samples; i++)
        {
            int features[7] = {1, 0, 0, 0, 0, 0, 0};
            board_to_features(model->dataset[i].board, features, 'x');
            int predicted_move = predict(model->dataset[i].board, *model, 'x');
            int error = predicted_move - model->dataset[i].outcome;
            total_loss += error * error;

            // weight update
            for (int j = 0; j < 7; j++)
            {
                model->weights[j] += model->learning_rate * (error * features[j] - 0.01 * model->weights[j]);
            }
        }

        total_loss /= num_samples;
        if (verbose)
        {
            for (int j = 0; j < 7; j++)
            {

                printf("%g ", model->weights[j]);
            }
            printf(" Epoch %d, Training loss for current epoch: %f\n", epoch, total_loss);
        }
    }
    printf("Training complete for Linear Regression Model!\n");
}

void evaluate(LinearRegressionModel *model)
{
    int num_samples = DATASET_SIZE * 0.2;
    int correct_predictions = 0;
    int outcomes[BOARD_TOTAL_SIZE][BOARD_TOTAL_SIZE] = {{0}};

    for (int i = DATASET_SIZE - num_samples; i < DATASET_SIZE; i++)
    {
        int features[7] = {1, 0, 0, 0, 0, 0, 0};
        board_to_features(model->dataset[i].board, features, 'x');
        double prediction = predict(model->dataset[i].board, *model, 'x');
        int predicted_move = (int)prediction;

        if (predicted_move == model->dataset[i].outcome)
        {
            correct_predictions++;
        }

        outcomes[model->dataset[i].outcome][predicted_move]++;
    }

    double accuracy = (double)correct_predictions / num_samples * 100;
    printf("Accuracy for testing set: %.2f%%\n", accuracy);

    printf("Confusion Matrix for testing data: \n");
    printf("Actual: 0   1   2   3   4   5   6   7   8\nP:\n");
    for (int i = 0; i < BOARD_TOTAL_SIZE; i++)
    {
        printf("%i      ", i);
        for (int j = 0; j < BOARD_TOTAL_SIZE; j++)
        {
            printf("%2i  ", outcomes[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void shuffle_data(MoveData *dataset, int num_samples)
{
    for (int i = 0; i < num_samples; i++)
    {
        int random = rand() % (i + 1);
        MoveData temp = dataset[i];
        dataset[i] = dataset[random];
        dataset[random] = temp;
    }
}

LinearRegressionModel *new_model(double learning_rate, int epochs)
{

    MoveData dataset[DATASET_SIZE];
    printf("Loading:\n");
    MoveData *ptr = dataset;
    if (load_dataset(ptr, "ml_algos/tictac_single.txt", DATASET_SIZE) != 0)
    {
        fprintf(stderr, "Dataset loading failed.\n");
        return NULL;
    }

    srand(time(NULL));                   // setting seed for random based on current time
    shuffle_data(dataset, DATASET_SIZE); // shuffling data for randomness
    LinearRegressionModel *model = (LinearRegressionModel *)malloc(sizeof(LinearRegressionModel));
    model->weights = (double *)malloc(BOARD_TOTAL_SIZE * sizeof(double));
    initialize_weights(model->weights);
    model->learning_rate = learning_rate;
    model->epochs = epochs;
    model->dataset = (MoveData *)malloc(DATASET_SIZE * sizeof(MoveData));
    for (int i = 0; i < DATASET_SIZE; i++)
    {
        model->dataset[i] = dataset[i];
    }

    return model;
}
