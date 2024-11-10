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

void initialize_weights(double weights[BOARD_SIZE], double weight)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        weights[i] = weight; // Random weights between 0 and 0.1
    }
}

double predict(double features[BOARD_SIZE], double weights[BOARD_SIZE], double bias)
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
        double score = bias;
        score += weights[i] * features[i];

        if (score > max_score)
        {
            max_score = score;
            best_move = i;
        }
    }
    return best_move;
}

void train(LinearRegressionModel *model)
{
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

            double prediction = predict(features, model->weights, *model->bias);
            int predicted_move = (int)roundf(prediction);
            int error = predicted_move - model->dataset[i].outcome;
            total_loss += error * error;

            // weight update
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                double adjustment = model->learning_rate * error * features[j]; // regularization
                model->weights[j] -= adjustment;

                // printf("Game %d:Readjusted weight for weight %d: %.2f\n", i, j, model->weights[j]);
            }
            *model->bias -= model->learning_rate * error;
        }

        total_loss /= num_samples;
        for (int j = 0; j < BOARD_SIZE; j++)
        {

            printf("%.2f ", model->weights[j]);

            // printf("Game %d:Readjusted weight for weight %d: %.2f\n", i, j, model->weights[j]);
        }
        printf(" Epoch %d, Training loss for current epoch: %f\n", epoch, total_loss);
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
        double prediction = predict(features, model->weights, *model->bias);
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
