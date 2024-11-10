# General steps for ML algorithm

### 1. Open data set file and extract data into multidimensional data array set.

The data will be extracted as 𝑥(m,n) where m is the data set row from 1 to 958 and n is the square box number of the tic-tac-toe game.

𝑛 = 1 : top-left-square: {x,o,b}

𝑛 = 2 : top-middle-square: {x,o,b}

𝑛 = 3 : top-right-square: {x,o,b}

𝑛 = 4 : middle-left-square: {x,o,b}

𝑛 = 5 : middle-middle-square: {x,o,b}

𝑛 = 6 : middle-right-square: {x,o,b}

𝑛 = 7 : bottom-left-square: {x,o,b}

𝑛 = 8 : bottom-middle-square: {x,o,b}

𝑛 = 9 : bottom-right-square: {x,o,b}

And y(m,1) is the classification output for each data row set, either positive or negative.

Split the data into 80:20 ratio of training to testing data.

#### Using the game data reader function:

1. Include it at the top of the file.
2. Load the data by calling the `load_dataset` function.
3. Compile by using the command as such:
   `gcc -o {EXECUTABLE_NAME} {YOUR C FILE NAME} dataset_loader.c move_data.h`

```c
#include "dataset_loader.h"
#include "move_data.h"
int  main()
{
	MoveData dataset[DATASET_SIZE];
    MoveData *ptr = dataset;

    if (load_dataset(ptr) != 0)
    {
        fprintf(stderr, "Dataset loading failed.\n");
        return 1;
    }
}
```

### 2. Refer to the project document or other references on how to use the data to train depending on the ML algorithm chosen.

Some options for ML algorithms:

- [Linear Regression](https://medium.com/analytics-vidhya/building-a-tictactoe-bot-with-regression-ba79f2ae74bb)

- [Navie Bayes](https://github.com/m-aminiz/NaiveBayes)

- [Neural Network](https://www.kaggle.com/code/dhanushkishore/a-self-learning-tic-tac-toe-program)

- Reinforcement Learning

### 3. Probability of Error Calculation

Calculate the probability of error for each training data and testing data.

For Confusion matrix, determine the four classes namely

- True Positive (TP): No of positive classifications that are predicted **correctly** (wins)

- True Negative (TN): No of negative classifications that are predicted **correctly** (lose)

- False Positive (FP): No of positive classifications that are predicted **wrongly** (win)

- False Negative (FN) : No of negative classifications that are predicted **wrongly** (lose)

https://www.dataschool.io/simple-guide-to-confusion-matrix-terminology/
