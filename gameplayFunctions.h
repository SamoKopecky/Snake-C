#include <stdbool.h>
#include <winnt.h>

struct twoDArray {
    int count;
    int **rows;
};

void
getValuesGameplayFunctions(HANDLE *ptrHStdout, char *ptrInputKey, bool *ptrGameOver, char *ptrDirection, int *ptrScore,
                           int ptrObstacleArray[52][2], const int *ptrObstacleArrayLength, int *ptrPlaygroundHeight,
                           int *ptrPlaygroundWidth, struct twoDArray *ptrSnakeCords, int ptrAppleCords[8][2],
                           int *ptrAppleCordsLength);

bool isApple(int x, int y);

bool isBorder(int x, int y);

bool isSnake(int x, int y);

bool isObstacle(int x, int y);

bool isSnakeBody(int x, int y);

void printSnake();

void changeDirection();

void updateSnake();

void updateScore();

void eatApple();
