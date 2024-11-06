#include <stdio.h>
#include <stdlib.h>
#include "data_reader.h"

int main()
{
    char **data = read_game_data();
    printf("Data from file:\n");
    for (int i = 0; i < 1000; i++)
    {
        printf("%s\n", data[i]);
    }

    return 0;
}