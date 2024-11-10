#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "dataset_loader.h"
#include "move_data.h"

#define DATASET_SIZE 958
#define BOARD_SIZE 9

double game_to_value(char cell);

double predict(double features[BOARD_SIZE], double weights[BOARD_SIZE], double bias);

void train(double weights[BOARD_SIZE], double *bias, MoveData *games, int num_samples, double learning_rate, int epochs);

void evaluate(double weights[BOARD_SIZE], double bias, MoveData *games, int num_samples);

void shuffle_data(MoveData *dataset, int num_samples);

int main()
{
    double weights[BOARD_SIZE] = {0.0}; // weight is 0 for all cells on the board.
    double bias = 0.0;
    double learning_rate = 0.01;
    int epochs = 1000; // how many times the model will go through the data set.
    MoveData dataset[DATASET_SIZE];
    MoveData *ptr = dataset;

    if (load_dataset(ptr, "tictac_single.txt", DATASET_SIZE) != 0)
    {
        fprintf(stderr, "Dataset loading failed.\n");
        return 1;
    }
    srand(time(NULL));
    shuffle_data(dataset, DATASET_SIZE); // shuffling data for randomness
    int training_samples = (int)(DATASET_SIZE * 0.8);
    int test_samples = DATASET_SIZE - training_samples;
    MoveData *training_data = dataset;
    MoveData *testing_data = dataset + training_samples; // use data after training samples

    train(weights, &bias, training_data, training_samples, learning_rate, epochs); // train model based on traits defined above

    evaluate(weights, bias, testing_data, test_samples);

    // do confusion matrix here
    return 0;
}

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

double predict(double features[BOARD_SIZE], double weights[BOARD_SIZE], double bias)
{
    double max_score = -1e9; // Initialize with a very low value
    int best_move = -1;

    for (int i = 0; i < 9; i++)
    {
        // Skip if the position is already occupied
        if (features[i] != 0)
        {
            continue;
        }

        // Calculate the prediction score for this position based on all features
        double score = bias;
        for (int j = 0; j < 9; j++)
        {
            score += weights[j] * features[j];
        }

        if (score > max_score)
        {
            max_score = score;
            best_move = i;
        }
    }

    return best_move;
}

void train(double weights[BOARD_SIZE], double *bias, MoveData *games, int num_samples, double learning_rate, int epochs)
{
    for (int epoch = 0; epoch < epochs; epoch++)
    {
        double total_loss = 0.0;

        for (int i = 0; i < num_samples; i++)
        {
            double features[BOARD_SIZE];
            board_to_features(games[i].board, features);
            double prediction = predict(features, weights, *bias);
            int predicted_move = (int)roundf(prediction);
            int error = predicted_move - games[i].outcome;
            // printf("Game %d: Predicted: %d Actual: %d\n", i, predicted_move, games[i].outcome);
            total_loss += error * error;

            // weight update
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                weights[j] -= learning_rate * error * features[j];
                // printf("Game %d:Readjusted weight for weight %d: %.2f\n", i, j, weights[j]);
            }
            *bias -= learning_rate * error;
        }

        // calculate total loss for current epoch
        total_loss /= num_samples;
        printf("Epoch %d, Training loss for current epoch: %f\n", epoch, total_loss);
    }
}

void evaluate(double weights[BOARD_SIZE], double bias, MoveData *games, int num_samples)
{
    int correct_predictions = 0;
    int predictions[BOARD_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    for (int i = 0; i < num_samples; i++)
    {
        double features[BOARD_SIZE];
        board_to_features(games[i].board, features);
        double prediction = predict(features, weights, bias);
        int predicted_move = (int)prediction;

        if (predicted_move == games[i].outcome)
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
