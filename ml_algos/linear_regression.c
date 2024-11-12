#include <stdio.h>
#include "linearRegressionModel.h"

int main()
{
    LinearRegressionModel *model = new_model(0.0, 0.0, 0.01, 10000); // 0.0 weight, 0.0 bias, 0.01 learning rate, 10000 epochs

    train(model, 0); // train model based on traits defined above

    evaluate(model);

    // do confusion matrix here
    return 0;
}
