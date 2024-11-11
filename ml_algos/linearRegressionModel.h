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

double game_to_value(char cell);

double predict(double features[BOARD_SIZE], double weights[BOARD_SIZE], double bias);

void train(LinearRegressionModel *model);

void evaluate(LinearRegressionModel *model);

void shuffle_data(MoveData *dataset, int num_samples);

LinearRegressionModel *new_model(double weights, double bias, double learning_rate, int epochs);

#endif