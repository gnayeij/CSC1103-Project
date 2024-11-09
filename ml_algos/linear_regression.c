#include <stdio.h>
#include <stdlib.h>
#include "dataset_loader.h"
#include "move_data.h"

#define DATASET_SIZE 958

int main()
{
    MoveData dataset[DATASET_SIZE];
    MoveData *ptr = dataset;

    if (load_dataset(ptr) != 0)
    {
        fprintf(stderr, "Dataset loading failed.\n");
        return 1;
    }

    printf("Data from file:\n");
    for (int i = 0; i < DATASET_SIZE; i++)
    {
        printf("Game Data %d: %s\n", i, dataset[i].board);
    }

    return 0;
}
