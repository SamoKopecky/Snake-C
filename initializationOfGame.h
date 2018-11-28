#include "gameplayFunctions.h"

void getPlayerName(struct playerInfo *playerInfo);

void printInstructions();

void getDifficulty(struct playerInput *playerInput);

char getCorrectCharacter(short x, short y, struct coordinates *coordinates);

char getCorrectBorder(short x, short y);

void updatePlayground(struct coordinates *coordinates, struct playerInfo *playerInfo);

void firstAlloc(struct coordinates *coordinates);

void importApples(struct coordinates *coordinates);