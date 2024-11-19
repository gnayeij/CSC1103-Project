#ifndef LINEAR_REGRESSION_MODEL_H
#define LINEAR_REGRESSION_MODEL_H

#include <stdio.h>
#include "move_data.h"

#define SIZE 3
// Define the dataset structure

typedef struct linearregressionmodel
{
    double *weights; // weight is 0 for all cells on the board.
    double *bias;
    double learning_rate;
    int epochs;
    MoveData *dataset;
} LinearRegressionModel;

int predict(char board[SIZE][SIZE], LinearRegressionModel model, char type);

void board_to_features(char board[SIZE][SIZE], int features[6], char type);

void train(LinearRegressionModel *model, int verbose);

void evaluate(LinearRegressionModel *model);

void shuffle_data(MoveData *dataset, int num_samples);

LinearRegressionModel *new_model(double bias, double learning_rate, int epochs);

#endif