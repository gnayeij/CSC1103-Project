#include <stdio.h>
#include "linearRegressionModel.h"

void printBoard(const char array[3][3])
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            // Print each cell in the 3x3 format
            printf(" %c ", array[i][j]);

            // Print a divider between columns
            if (j < 2)
            {
                printf("|");
            }
        }

        // Print a divider between rows
        if (i < 2)
        {
            printf("\n---|---|---\n");
        }
    }
    printf("\n");
}

int main()
{
    LinearRegressionModel *model = new_model(1.0, 0.0, 0.001, 100); // 0.0 weight, 0.0 bias, 0.01 learning rate, 10000 epochs

    train(model, 1); // train model based on traits defined above

    evaluate(model);

    char board[3][3] = {{"bbo"}, {"box"}, {"xbx"}};
    int prediction = predict(board, *model, 'x');
    // print board
    printBoard(board);
    printf("prediction for board: %d", prediction);

    // do confusion matrix here
    return 0;
}
