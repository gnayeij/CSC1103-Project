#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dataset_loader.h"
#include "move_data.h"

#define DATASET_LENGTH 958
#define DATA_FILE "tic-tac-toe.txt"

int load_dataset(MoveData *dataset)
{
    FILE *file = fopen(DATA_FILE, "r");
    if (!file)
    {
        perror("Error opening file");
        return -1;
    }

    for (int i = 0; i < DATASET_LENGTH; i++)
    {
        if (fscanf(file, "%s %d", dataset->board, &(dataset->outcome)) != 2)
        {
            fprintf(stderr, "Error reading line %d\n", i);
            break; // Exit loop if reading fails
        }
        dataset++; // Move to the next struct
    }

    fclose(file);
    return 0;
}
