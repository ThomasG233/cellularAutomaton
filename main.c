#include <stdio.h>
#include <stdlib.h>
#include "grid.h"
#include <ctype.h> // for toupper() used in send to file
#include <stdbool.h>
#include <string.h>

int main()
{
    int binary[8] = {0,0,0,0,0,0,0,0};
    int *binPtr = binary;
    int ruleset = 0;
    int numOfGens = 0;
    char fileName[30] = {'\0'};
    if(collectGenerationInfo(&ruleset, &numOfGens) != 0)
    {
        printf("Invalid generation information. \n");
    }
    else
    {
        convertRulesetToBinary(ruleset, binPtr);
        sendToFile(fileName);
        generateAutomaton(binPtr, numOfGens, fileName);
    }
}

int sendToFile(char fileName[]){
    char check;
    printf("Would you like to send the output to a file? (Y/N): ");
    scanf(" %c", &check);
    if(check=='Y'){
        printf("Enter the name of a file to output to: ");
        scanf("%s", fileName);
    } 
    return 1;
}

int generateAutomaton(int *binaryRuleset, int numberOfGens, char fileName[])
{
    // Create a Grid pointer and allocate memory.
    GridGenerations *gPtr;
    gPtr = (GridGenerations*)malloc(sizeof(GridGenerations));
    bool firstLine = true;
    // If memory was successfully allocated.
    if(gPtr != NULL)
    {
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
                    if(gPtr->previousRow[i-1] == 1 && gPtr->previousRow[i] == 1 && gPtr->previousRow[i+1] == 1)
                    {
                        gPtr->newRow[i] = binaryRuleset[0];
                    }
                    else if(gPtr->previousRow[i-1] == 1 && gPtr->previousRow[i] == 1 && gPtr->previousRow[i+1] == 0)
                    {
                        gPtr->newRow[i] = binaryRuleset[1];
                    }
                    else if(gPtr->previousRow[i-1] == 1 && gPtr->previousRow[i] == 0 && gPtr->previousRow[i+1] == 1)
                    {
                        gPtr->newRow[i] = binaryRuleset[2];
                    }
                    else if(gPtr->previousRow[i-1] == 1 && gPtr->previousRow[i] == 0 && gPtr->previousRow[i+1] == 0)
                    {
                        gPtr->newRow[i] = binaryRuleset[3];
                    }
                    else if(gPtr->previousRow[i-1] == 0 && gPtr->previousRow[i] == 1 && gPtr->previousRow[i+1] == 1)
                    {
                        gPtr->newRow[i] = binaryRuleset[4];
                    }
                    else if(gPtr->previousRow[i-1] == 0 && gPtr->previousRow[i] == 1 && gPtr->previousRow[i+1] == 0)
                    {
                        gPtr->newRow[i] = binaryRuleset[5];
                    }
                    else if(gPtr->previousRow[i-1] == 0 && gPtr->previousRow[i] == 0 && gPtr->previousRow[i+1] == 1)
                    {
                        gPtr->newRow[i] = binaryRuleset[6];
                    }
                    else
                    {
                        gPtr->newRow[i] = binaryRuleset[7];
                    }
                }
                printRow(gPtr->newRow);

                // Name will only be empty if the user chose not to save to file.
                if (strlen(fileName)!=0){
                    FILE *fp;
                    // Overwrite any existing info on first line, then append on subsequent loops
                    if(firstLine==true){
                        fp = fopen(fileName, "w");
                        firstLine=false;
                    } else {
                        fp = fopen(fileName, "a");
                    }
                    if(fp!=NULL){
                        writeRow(fp, gPtr->previousRow);
                    }
                    fclose(fp);
                }

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
            return 1;
        }
        free(gPtr);
    }
    else // if memory couldn't be allocated.
    {
        printf("Could not allocate memory successfully; no grid can be created.\n");
        return 1;
    }
    return 0;
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

void writeRow(FILE *fp,int row[]){
    if (fp!=NULL){
        for(int i = 1; i < GRID_SIZE-1; i++){
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
    int bits[8] = {128, 64, 32, 16, 8, 4, 2, 1};
    printf("IN: Ruleset %d as binary: ", number);
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
