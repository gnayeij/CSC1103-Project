#include <stdio.h>
#include "linearRegressionModel.h"

// void printBoard(const char *gameData)
// {
//     for (int i = 0; i < 9; i++)
//     {
//         // Print each cell in the 3x3 format
//         printf(" %c ", gameData[i]);

//         // Print a divider between columns
//         if (i % 3 != 2)
//         {
//             printf("|");
//         }

//         // Print a divider between rows
//         if (i % 3 == 2 && i < 8)
//         {
//             printf("\n---|---|---\n");
//         }
//     }
//     printf("\n");
// }

int main()
{
    LinearRegressionModel *model = new_model(0.0, 0.0, 0.01, 10000); // 0.0 weight, 0.0 bias, 0.01 learning rate, 10000 epochs

    train(model, 1); // train model based on traits defined above

    evaluate(model);

    char board[3][3] = {{"bbo"},{"box"},{"xbx"}};
    // int prediction = predict(board, *model, 'x');
    // // print board
    // printBoard(board);
    // // printf("prediction for board: %d", prediction);

    // double features[6] = {1};
    // board_to_features_new(board, features, 'x');

    // do confusion matrix here
    return 0;
}
