#include <stdio.h>      // standard I/O functions for printing and reading input
#include <stdbool.h>    // True / False functions
#include <unistd.h>     // access to sleep() functions used for delays

#define clear "\e[1;1H\e[2J"    // clears the screen using an ANSI escape sequence

void grid_print(char(*)[4]);    // prints the game board
void human(char(*)[4]);         // handles human player moves
void computer(char (*)[4]);     // handles computer moves
int check(char (*)[4]);         // checks the current state of game for any winner
int minimax(char (*)[4],int);   // algorithm computer uses to play optimally

int main()
{
    bool player=1;      // player starts
    int i;

    char grid[4][4]={"   ","   ","   "},names[2][10]={"COMPUTER","HUMAN"};

    grid_print(grid);       // print empty grid

    for (i=0;i<9;i++)
    {
        if(player)
            human(grid);        // player move
         else
            computer(grid);     // computer move
        
        grid_print(grid);       // print the grid for each move

        if (check(grid) == 1 || check(grid) == -1)      // check for winner
        {
            printf("%s IS THE WINNER!\n",names[player]);        // print whoever is the winner
            break;
        }
        player = !player;       // switch turns
    }

    if (i == 9)     // if all grids are filled and no winner, DRAW
        printf("GAME DRAW ");
}

void computer(char (*grid)[4])
{
    int i,j,pt,bpt=-20,index_x,index_y;

    for (i=0;i<3;i++)       // Loop through grid positions
    {
        for (j=0;j<3;j++)
        {
            if (grid[i][j] == ' ')      // if cell is empty
            {
                grid[i][j]='O';         // Try the move fot computer
                pt=minimax(grid,0);     // Get the score using minimax
                grid[i][j]=' ';         // Undo the move

                if(pt>bpt)              // Choose the best possible move
                {
                    bpt=pt;
                    index_x=i;
                    index_y=j;
                }
            }
        }
    }
    grid[index_x][index_y]='O';
}

int minimax(char (*grid)[4],int ismax)
{
    int score,i,j,pt;
    score = check(grid);        // check if the game has ended
    
    if (score != 0)             // if there is a winner or draw
    {
        return score;
    }

    if (ismax)      // maximizer (computer turn)
    {
        int bpt = -1;

        for (i=0;i<3;i++)
        {
            for (j=0;j<3;j++)
            {
                if (grid[i][j] == ' ')
                {
                    grid[i][j] = 'O';           // Try the move
                    pt = minimax(grid,0);       // Get the score
                    grid[i][j] = ' ';           // Undo the move

                    if (pt>bpt)
                    {
                        bpt = pt;
                    }
                }
            }
        }
        return bpt;
    }

    else        // minimizer (player turn)
    {
        int bpt = 1;

        for (i=0;i<3;i++)
        {
            for (j=0;j<3;j++)
            {
                if (grid[i][j] == ' ')
                {
                    grid[i][j] = 'X';           // Try the move
                    pt = minimax(grid,1);       // Get the score
                    grid[i][j] = ' ';           // Undo the move

                    if (pt<bpt)
                    {
                        bpt = pt;
                    }
                }
            }
        }
        return bpt;
    }
}

void grid_print(char (*grid)[4])
{
    printf(clear);  // clear the screen
    sleep(1);       // sleep for 1 second to create a delay between moves

    printf("  %c  |  %c  |  %c  \n",grid[0][0],grid[0][1],grid[0][2]);
    printf("-----------------\n");
    printf("  %c  |  %c  |  %c  \n",grid[1][0],grid[1][1],grid[1][2]);
    printf("-----------------\n");
    printf("  %c  |  %c  |  %c  \n",grid[2][0],grid[2][1],grid[2][2]);
}

void human(char (*grid)[4])
{
    int n,ch[10]={0,00,01,02,10,11,12,20,21,22},index,index_x,index_y;
    
    A:
    printf("\nCHOOSE YOUR BOX NUMBER ");
    scanf("%d",&n);
    
    index=ch[n];
    index_x=index/10;
    index_y=index%10;

    if (index_x>2 || index_y>2 || grid[index_x][index_y]!=' ')
    {
        printf("INVALID INPUT!\n");
        goto A;     // Prompt for input again if invalid
    }

    grid[index_x][index_y]='X';     // player move is 'X'
}

int check(char (*grid)[4])
{
    int c=0;        //counter for filled cells

    for (int i=0;i<3;i++)
    {
        // check for horizontal
        if ( (grid[i][0] == grid[i][1]) && (grid[i][1] == grid[i][2]) && (grid[i][0] != ' ') )
        {
            if (grid[i][0] == 'X')
                return -1;      // player wins
            else
                return 1;       // computer wins
        }

        // check for vertical
        if ( (grid[0][i] == grid[1][i]) && (grid[1][i] == grid[2][i]) && (grid[0][i] != ' ') )
        {
            if (grid[0][i] == 'X')
                return -1;      // player wins
            else
                return 1;       // computer wins
        }
    }

    // check for diagonal
    if ( ((grid[0][0] == grid[1][1]) && (grid[1][1] == grid[2][2]) && (grid[0][0] != ' ')) || ((grid[0][2] == grid[1][1]) && (grid[1][1] ==grid[2][0]) && (grid[1][1] != ' ')) )
    {
        if (grid[1][1] == 'X')
            return -1;      // player wins
        else
            return 1;       // computer wins
    }

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if(grid[i][j] != ' ')
                c++;        // increment counter for filled cells
        } 
    }
    if(c == 9)
        return 78;      //random number indicating its a tie
    else
        return 0;       //number telling the game is still ongoing
}