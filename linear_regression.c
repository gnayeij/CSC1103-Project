#include <stdio.h>
#include "ml_algos/linearRegressionModel.h"

int main()
{
    LinearRegressionModel *model = new_model(0.001, 10000); // 0.01 learning rate, 10000 epochs

    train(model, 1); // train model based on traits defined above

    evaluate(model);

    // do confusion matrix here
    return 0;
}
