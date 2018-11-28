#include <afxres.h>
#include <time.h>
#include <stdbool.h>
#include "fileFunctions.h"

bool isApple(short x, short y, struct coordinates *coordinates);

bool isBorder(short x, short y);

bool isSnake(short x, short y, struct coordinates *coordinates);

bool isObstacle(short x, short y, struct coordinates *coordinates);

bool isSnakeBody(short x, short y, struct coordinates *coordinates);

void printSnake(struct coordinates *coordinates, HANDLE *hStdout);

void changeDirection(struct playerInput *playerInput);

void updateSnake(struct coordinates *coordinates, struct playerInput *playerInput, bool *gameOver);

void updateScore(HANDLE *hStdout, struct playerInfo *playerInfo);

void eatApple(struct coordinates *coordinates, HANDLE *hStdout, struct playerInfo *playerInfo);
