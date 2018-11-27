#include "gameplayFunctions.h"

void
getValuesInitializationOfGame(char ptrPlayerName[10], int *ptrDiff, int *ptrPlaygroundHeight, int *ptrPlaygroundWidth,
                              char ptrPlayground[25][50], int *ptrScore, struct twoDArray *ptrSnakeCords,
                              int ptrAppleCords[8][2], int *ptrAppleCordsLength);

void getPlayerName();

void printInstructions();

void getDifficulty();

char getCorrectCharacter(int x, int y);

char getCorrectBorder(int x, int y);

void updatePlayground();

void firstAlloc();

void importApples();