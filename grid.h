#include <stddef.h>

#define GRID_SIZE 31

typedef struct gridGenerations {
    int previousRow[GRID_SIZE];
    int newRow[GRID_SIZE];
} GridGenerations;

int initialiseRows(GridGenerations *gridPtr);
void printRow(int row[]);
int generateAutomaton();
int collectGenerationInfo(int *ruleset, int *genNumber);
int convertRulesetToBinary(int number, int *binary);
