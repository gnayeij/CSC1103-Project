#ifndef LINEAR_REGRESSION_MODEL_H
#define LINEAR_REGRESSION_MODEL_H

#include <stdio.h>
#include "move_data.h"

#define BOARD_SIZE 9
// Define the dataset structure

typedef struct linearregressionmodel
{
    double *weights; // weight is 0 for all cells on the board.
    double *bias;
    double learning_rate;
    int epochs;
    MoveData *dataset;
} LinearRegressionModel;

int predict(char board[BOARD_SIZE], LinearRegressionModel model);

void train(LinearRegressionModel *model, int verbose);

void evaluate(LinearRegressionModel *model);

void shuffle_data(MoveData *dataset, int num_samples);

LinearRegressionModel *new_model(double weights, double bias, double learning_rate, int epochs);

#endif