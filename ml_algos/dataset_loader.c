#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dataset_loader.h"
#include "move_data.h"

int load_dataset(MoveData *dataset, char file_name[], int DATASET_LENGTH)
{
    FILE *file = fopen(file_name, "r");
    if (!file)
    {
        perror("Error opening file");
        return -1;
    }

    for (int i = 0; i < DATASET_LENGTH; i++)
    {        
        char temp[10]=" ";
        
        // Read a 9-character board state and an integer outcome
        int read_count = fscanf(file, "%s %d", temp, &dataset[i].outcome);
                
        // Check if fscanf read both values correctly
        if (read_count != 2)
        {
            fprintf(stderr, "Error reading line %d\n", i);
            break;  // Exit loop if reading fails
        }
        // set board
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                dataset[i].board[j][k] = temp[j*3+k];
                
            }
        }

    }

    fclose(file);
    return 0;
}
