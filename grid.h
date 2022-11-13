#include <stddef.h>
#define GRID_SIZE 31

typedef struct gridGenerations {
    char previousRow[GRID_SIZE];
    char newRow[GRID_SIZE];
    int binary[8];
    int ruleset;
    int numOfGens;
} GridGenerations;

int initialiseGrid(GridGenerations *gridPtr);
void printRow(char row[]);
int generateAutomaton(GridGenerations *gPtr, char fileName[]);
int collectGenerationInfo(int *ruleset, int *genNumber);
int convertRulesetToBinary(int number, int *binary);
int convertRulesetToDecimal(int binary[], int *number);
int sendToFile();
void writeRow();
