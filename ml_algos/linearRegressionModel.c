#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "dataset_loader.h"
#include "move_data.h"
#include "linearRegressionModel.h"

#define DATASET_SIZE 958
#define BOARD_SIZE 9

double game_to_value(char cell)
{
    switch (cell)
    {
    case 'x':
        return 1.0;

    case 'o':
        return -1.0;

    default:
        return 0.0;
    }
}

void board_to_features(char board[BOARD_SIZE], double features[BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        features[i] = game_to_value(board[i]);
    }
}

void board_to_features_new(char board[BOARD_SIZE], double features[6], char type)
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
    char opponent_type = 'o';
    if (type == 'o')
    {
        opponent_type = 'x';
    }

    const char own_shape = type;
    int own_rows = 0, own_cols = 0, own_diag = 0, en_rows = 0, en_cols = 0, en_diag = 0, emp_rows = 0, emp_cols = 0, emp_diag = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (board[i] == type)
        {
            own_cols++, own_rows++;

            // corner check
            if (i == 0 || i == 2 || i == 6 || i == 8)
            {
                features[4]++;
                own_diag++;
            }
            // centre piece check

            if (i == 4)
            {
                features[3]++;
                own_diag++;
            }
        }
        else if (board[i] == opponent_type)
        {
            en_cols++, en_rows++;
            // diag check
            if (i % 2 == 0)
            {
                en_diag++;
            }
        }
        else
        {
            emp_cols++, emp_rows++;
            if (i % 2 == 0)
            {
                emp_diag++;
            }
        }
    }

    //
    printf("own_rows:%d own_cols:%d own_diag:%d en_rows:%d en_cols:%d en_diag:%d emp_rows:%d emp_cols:%d emp_diag:%d \n", own_rows, own_cols, own_diag, en_rows, en_cols, en_diag, emp_rows, emp_cols, emp_diag);
}

void initialize_weights(double weights[BOARD_SIZE], double weight)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        weights[i] = weight; // Random weights between 0 and 0.1
    }
}

double predict_wrap(double features[BOARD_SIZE], double weights[BOARD_SIZE], double bias)
{
    double max_score = -1e9;
    int best_move = -1;

    for (int i = 0; i < 9; i++)
    {
        // skip if its not blank
        if (features[i] != 0)
        {
            continue;
        }

        // calculate score based off all weights and features here TODO: verify if selected option is a valid move
        // printf("weight for feature %d: %.4f : %.4f\n", i, weights[i], features[i]);

        double score = bias + weights[i];

        if (score > max_score)
        {
            max_score = score;
            best_move = i;
        }
        // printf("Score for feature %d: %.2f\n", i, score);
    }
    return best_move;
}

int predict(char board[BOARD_SIZE], LinearRegressionModel model, char type)
{
    double features[BOARD_SIZE] = {0};
    board_to_features(board, features);
    double prediction = predict_wrap(features, model.weights, *model.bias);
    int predicted_move = (int)roundf(prediction);
    return predicted_move;
}

int predict_new(char board[BOARD_SIZE], LinearRegressionModel model, char type)
{
    double features[6] = {1};
    board_to_features_new(board, features, type);
}

void train(LinearRegressionModel *model, int verbose)
{
    printf("Starting training with following parameters: Weight: %.2f, Bias: %.2f, Learning Rate: %.2f, Epochs: %d...\n", model->weights, model->bias, model->learning_rate, model->epochs);
    int num_samples = DATASET_SIZE * 0.8;

    for (int epoch = 0; epoch < model->epochs; epoch++)
    {

        double total_loss = 0.0;
        // shuffle dataset
        shuffle_data(model->dataset, DATASET_SIZE);
        for (int i = 0; i < num_samples; i++)
        {
            double features[BOARD_SIZE] = {0};
            board_to_features(model->dataset[i].board, features);

            int predicted_move = predict(model->dataset[i].board, *model, 'x');
            int error = predicted_move - model->dataset[i].outcome;
            total_loss += error * error;

            // weight update
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                if (features[j] == 0)
                {
                    double adjustment = model->learning_rate * error;
                    model->weights[j] -= adjustment;
                }
            }
            *model->bias -= model->learning_rate * error;
            // printf("Bias for epoch %d: %.2f, minused: %.2f\n", epoch, *model->bias, model->learning_rate * error);
        }

        total_loss /= num_samples;
        if (verbose)
        {
            for (int j = 0; j < BOARD_SIZE; j++)
            {

                printf("%.2f ", model->weights[j]);
            }
            printf(" Epoch %d, Training loss for current epoch: %f\n", epoch, total_loss);
        }
    }
}

void evaluate(LinearRegressionModel *model)
{
    int num_samples = DATASET_SIZE * 0.2;
    int correct_predictions = 0;
    int predictions[BOARD_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    for (int i = DATASET_SIZE - num_samples; i < DATASET_SIZE; i++)
    {
        double features[BOARD_SIZE] = {0};
        board_to_features(model->dataset[i].board, features);
        double prediction = predict_wrap(features, model->weights, *model->bias);
        int predicted_move = (int)prediction;

        if (predicted_move == model->dataset[i].outcome)
        {
            correct_predictions++;
        }
        predictions[predicted_move]++;
    }
    double accuracy = (double)correct_predictions / num_samples * 100;
    printf("Accuracy for testing set: %.2f%%\n", accuracy);
    printf("Number of picks per slot: ");
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        printf("%d:%d ", i, predictions[i]);
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

LinearRegressionModel *new_model(double weight, double bias, double learning_rate, int epochs)
{

    MoveData dataset[DATASET_SIZE];
    MoveData *ptr = dataset;
    if (load_dataset(ptr, "tictac_single.txt", DATASET_SIZE) != 0)
    {
        fprintf(stderr, "Dataset loading failed.\n");
        return NULL;
    }

    srand(time(NULL));

    shuffle_data(dataset, DATASET_SIZE); // shuffling data for randomness
    LinearRegressionModel *model = (LinearRegressionModel *)malloc(sizeof(LinearRegressionModel));
    model->weights = (double *)malloc(BOARD_SIZE * sizeof(double));
    initialize_weights(model->weights, weight);
    model->bias = (double *)malloc(sizeof(double));
    *(model->bias) = bias;
    model->learning_rate = learning_rate;
    model->epochs = epochs;
    model->dataset = (MoveData *)malloc(DATASET_SIZE * sizeof(MoveData));
    for (int i = 0; i < DATASET_SIZE; i++)
    {
        model->dataset[i] = dataset[i];
    }

    return model;
}
