// file_reader.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_reader.h"

#define MAX_LENGTH 100

char **read_game_data()
{
    FILE *file = fopen("tic-tac-toe.data", "r");
    int line_count = 0;
    if (!file)
    {
        printf("\nFailed to open file");
        return NULL;
    }

    // Determine the number of lines in the file
    char temp[MAX_LENGTH];
    while (fgets(temp, MAX_LENGTH, file) != NULL)
    {
        line_count++;
    }
    rewind(file); // Reset file pointer to the beginning

    // Allocate memory for data
    char **data = (char **)malloc(line_count * sizeof(char *));
    if (!data)
    {
        printf("\nFailed to allocate memory");
        fclose(file);
        line_count = 0;
        return NULL;
    }

    // Read each line and allocate to arrays
    for (int i = 0; i < line_count; i++)
    {
        char line[MAX_LENGTH];
        if (fgets(line, MAX_LENGTH, file))
        {
            line[strcspn(line, "\n")] = '\0';           // remove \n at the end of line
            data[i] = (char *)malloc(strlen(line) + 1); // set size of string
            if (data[i])
            {
                strcpy(data[i], line);
            }
        }
        else
        {
            data[i] = NULL;
        }
    }

    fclose(file);
    return data;
}
