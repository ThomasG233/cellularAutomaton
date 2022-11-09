#include <stdio.h>
#include <stdlib.h>
#include "grid.h"

int main()
{
    // Create a Grid pointer and allocate memory.
    GridGenerations *gPtr;
    gPtr = (GridGenerations*)malloc(sizeof(GridGenerations));
    // If memory was successfully allocated.
    if(gPtr != NULL)
    {
        int numberOfGens = 14;
        // If the grid was successfully initialised
        if(initialiseRows(gPtr) == 0)
        {
            // Basic test to see if it generates like in the video.
            gPtr->previousRow[15] = 1;
            printRow(gPtr->previousRow);
            // Repeat automaton for each new iteration of the grid.
            for(int j = 0; j < numberOfGens; j++)
            {
                for(int i = 1; i < GRID_SIZE; i++)
                {
                    // Checks to see if ruleset applies.
                    if((gPtr->previousRow[i-1] == 1 && gPtr->previousRow[i] == 1 && gPtr->previousRow[i+1] == 1) 
                        || (gPtr->previousRow[i-1] == 1 && gPtr->previousRow[i] == 1 && gPtr->previousRow[i+1] == 0)
                        || (gPtr->previousRow[i-1] == 1 && gPtr->previousRow[i] == 0 && gPtr->previousRow[i+1] == 1)
                        || (gPtr->previousRow[i-1] == 0 && gPtr->previousRow[i] == 0 && gPtr->previousRow[i+1] == 0))
                    {
                        gPtr->newRow[i] = 0;
                    }
                    else
                    {
                        gPtr->newRow[i] = 1;
                    }
                }
                printRow(gPtr->newRow);
                // Copy old generation to the new generation.
                for(int i = 1; i < GRID_SIZE; i++)
                {
                    gPtr->previousRow[i] = gPtr->newRow[i];
                }
            }
        }
        else // if values were not successfull initialised.
        {
            printf("Could not initialise grid values successfully.\n");
        }
    free(gPtr);
    }
    else // if memory couldn't be allocated.
    {
        printf("Could not allocate memory successfully; no grid can be created.\n");
    }
}
// Set all rows of a grid object to 0.
int initialiseRows(GridGenerations *gridPtr)
{   
    if(gridPtr == NULL)
    {
        return 1;
    }

    for(int i = 0; i < GRID_SIZE; i++)
    {
        gridPtr->previousRow[i] = 0;
        gridPtr->newRow[i] = 0;
    }
    return 0;
}
// Print out a given row.
void printRow(int row[])
{
    for(int i = 1; i < GRID_SIZE-1; i++)
    {
        printf("|");    
        if(row[i] == 1)
        {
            printf("█");
        }
        else
        {
            printf(" ");
        }
    }
    printf("|\n");
}
