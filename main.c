#include <stdio.h>
#include <stdlib.h>
#include "grid.h"
#include <ctype.h> // for toupper() used in send to file
#include <stdbool.h>
#include <string.h>

int main()
{   
    char fileName[30] = {'\0'};
    GridGenerations *gPtr = (GridGenerations*)malloc(sizeof(GridGenerations));
    if(initialiseGrid(gPtr) == 1)
    {
        printf("Could not allocate memory to a generation successfully.");
    }
    else
    {
        if(collectGenerationInfo(&gPtr->ruleset, &gPtr->numOfGens) != 0)
        {
            printf("Invalid generation information. \n");
        }
        else
        {
            if(convertRulesetToBinary(gPtr->ruleset, gPtr->binary) == 0)
            {
                sendToFile(fileName);
                generateAutomaton(gPtr, fileName);
            }
            else
            {
                printf("Could not convert to binary correctly.");
            }
        }
        free(gPtr);
    }    
}

int sendToFile(char fileName[])
{
    char check;
    printf("Would you like to send the output to a file? (Y/N): ");
    scanf(" %c", &check);
    if(check=='Y'){
        printf("Enter the name of a file to output to: ");
        scanf("%s", fileName);
    } 
    return 1;
}

void writeRow(FILE *fp, char row[])
{
    if(fp!=NULL)
    {
        for(int i = 1; i < GRID_SIZE-1; i++)
        {
            fprintf(fp, "|");
            if(row[i] == 1)
            {
                fprintf(fp, "█");
            }
            else
            {
                fprintf(fp, " ");
            }
        }
        fprintf(fp, "|\n");
    }
}

int generateAutomaton(GridGenerations *gPtr, char fileName[])
{
    bool firstLine = true;
    // If memory was successfully allocated.
    if(gPtr != NULL)
    {
         // Basic test to see if it generates like in the video.
        gPtr->previousRow[15] = 1;
        printRow(gPtr->previousRow);
        // Repeat automaton for each new iteration of the grid.
        for(int j = 0; j < gPtr->numOfGens; j++)
        {
            for(int i = 1; i < GRID_SIZE-1; i++)
            {
                // Checks to see if ruleset applies.
                if(gPtr->previousRow[i-1] == 1 && gPtr->previousRow[i] == 1 && gPtr->previousRow[i+1] == 1)
                {
                    gPtr->newRow[i] = gPtr->binary[0];
                }
                else if(gPtr->previousRow[i-1] == 1 && gPtr->previousRow[i] == 1 && gPtr->previousRow[i+1] == 0)
                {
                    gPtr->newRow[i] = gPtr->binary[1];
                }
                else if(gPtr->previousRow[i-1] == 1 && gPtr->previousRow[i] == 0 && gPtr->previousRow[i+1] == 1)
                {
                    gPtr->newRow[i] = gPtr->binary[2];
                }
                else if(gPtr->previousRow[i-1] == 1 && gPtr->previousRow[i] == 0 && gPtr->previousRow[i+1] == 0)
                {
                    gPtr->newRow[i] = gPtr->binary[3];
                }
                else if(gPtr->previousRow[i-1] == 0 && gPtr->previousRow[i] == 1 && gPtr->previousRow[i+1] == 1)
                {
                    gPtr->newRow[i] = gPtr->binary[4];
                }
                else if(gPtr->previousRow[i-1] == 0 && gPtr->previousRow[i] == 1 && gPtr->previousRow[i+1] == 0)
                {
                    gPtr->newRow[i] = gPtr->binary[5];
                }
                else if(gPtr->previousRow[i-1] == 0 && gPtr->previousRow[i] == 0 && gPtr->previousRow[i+1] == 1)
                {
                    gPtr->newRow[i] = gPtr->binary[6];
                }
                else
                {
                    gPtr->newRow[i] = gPtr->binary[7];
                }
            }

            // Name will only be empty if the user chose not to save to file.
            if(strlen(fileName) != 0)
            {
                FILE *fp;
                // Overwrite any existing info on first line, then append on subsequent loops
                if(firstLine==true)
                {
                    fp = fopen(fileName, "w");
                    firstLine=false;
                } 
                else 
                {
                    fp = fopen(fileName, "a");
                }
                if(fp!=NULL)
                {
                    writeRow(fp, gPtr->previousRow);
                }
                fclose(fp);
            }
            printRow(gPtr->newRow);
            // Copy old generation to the new generation.
            for(int i = 1; i < GRID_SIZE-1; i++)
            {
                gPtr->previousRow[i] = gPtr->newRow[i];
            }
        }
        return 0;
    }
    else
    {
        return 1;
    }
}
// Set all rows of a grid object to 0.
int initialiseGrid(GridGenerations *gridPtr)
{   
    if(gridPtr == NULL)
    {
        return 1;
    }

    for(int i = 0; i < GRID_SIZE; i++)
    {
        gridPtr->previousRow[i] = 0;
        gridPtr->newRow[i] = 0;
        if(i < 8)
        {
            gridPtr->binary[i] = 0;
        }
    }

    gridPtr->ruleset = -1;
    gridPtr->numOfGens = -1;

    return 0;
}

// Print out a given row.
void printRow(char row[])
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

int collectGenerationInfo(int *ruleset, int *genNumber)
{
    printf("Enter the number of generations you wish to create: ");
    scanf("%d", genNumber);
    if(*genNumber < 1 || *genNumber > 30)
    {
        return 1;
    }
    printf("Enter the ruleset number between 0 and 255 (e.g. 30): ");
    scanf("%d", ruleset);
    if(*ruleset < 0  || *ruleset > 255)
    {
        return 1;
    }
    return 0;
}

int convertRulesetToBinary(int number, int *binary)
{
    if(binary == NULL)
    {
        return 1;
    }
    
    int bits[8] = {128, 64, 32, 16, 8, 4, 2, 1};
    
    printf("Ruleset %d as binary: ", number);
    for(int i = 0; i < 8; i++)
    {
        if((number % bits[i] != number))
        {
            binary[i] = 1;
            number %= bits[i];
        }
        else
        {
            binary[i] = 0;
        }
        printf("%d", binary[i]);
    }
    printf("\n");
    return 0;
}

int convertRulesetToDecimal(int binary[], int *number)
{
    if(number == NULL)
    {
        return 1;
    }
    int bits[8] = {128, 64, 32, 16, 8, 4, 2, 1};
    printf("Ruleset %d as binary: ", *number);
    for(int i = 0; i < 8; i++)
    {
        printf("%d", binary[i]);
        if((binary[i] == 1))
        {
            *number = *number + bits[i];
        }
    }
    printf("\nAs decimal: %d\n", *number);
    return 0;
}

