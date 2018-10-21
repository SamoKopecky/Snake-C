#include <stdio.h>
#include <stdbool.h>
#include <time.h>

char playground[25][50];
int playgroundHeight = sizeof(playground) / sizeof(playground[0]);
int playgroundWidth = sizeof(playground[0]) / sizeof(playground[0][0]);
int snakeCords[5][2] = {{2, 2},
                        {2, 3},
                        {2, 4},
                        {2, 5},
                        {2, 6}};
int appleCords[1][2] = {{2, 8}};
int delay = 1;

char checkForPlaygroundBorder(int x, int y) {
    if (x == 0) {
        return 'u';
    }
    if (x == playgroundHeight - 1) {
        return 'd';
    }
    if (y == 0) {
        return 'l';
    }
    if (y == playgroundWidth - 1) {
        return 'r';
    }
    return 'f';
}

void checkForAppleForSnake() {
    int snakeHead[2] = {snakeCords[sizeof(snakeCords) / sizeof(snakeCords[0]) - 1][0],
                        snakeCords[sizeof(snakeCords) / sizeof(snakeCords[0]) - 1][1]};

    //printf("x%d, y:%d", snakeHead[0], snakeHead[1]);
    for (int i = 0; i < sizeof(appleCords) / sizeof(appleCords[0]); ++i) {
        if (snakeHead[0] == appleCords[i][0] && snakeHead[1] == appleCords[i][1]) {
            // TODO
        }
    }
}

bool checkForAppleForPlayground(int x, int y) {
    for (int i = 0; i < sizeof(appleCords) / sizeof(appleCords[0]); ++i) {
        if (appleCords[i][0] == x && appleCords[i][1] == y) {
            return true;
        }
        return false;
    }
}

void checkForSnakeWrap(int i) {
    //printf("%d-", snakeCords[i][0]);
    //printf("%d ", snakeCords[i][1]);
    //printf("%c", checkForPlaygroundBorder(snakeCords[i][0], snakeCords[i][1]));
    if (checkForPlaygroundBorder(snakeCords[i][0], snakeCords[i][1]) == 'r') {
        snakeCords[i][1] = 1;
    }
    if (checkForPlaygroundBorder(snakeCords[i][0], snakeCords[i][1]) == 'l') {
        snakeCords[i][1] = playgroundWidth - 2;
    }
    if (checkForPlaygroundBorder(snakeCords[i][0], snakeCords[i][1]) == 'd') {
        snakeCords[i][0] = 1;
    }
    if (checkForPlaygroundBorder(snakeCords[i][0], snakeCords[i][1]) == 'u') {
        snakeCords[i][0] = playgroundHeight - 2;
    }

}

void printPlayground() {
    for (int x = 0; x < playgroundHeight; ++x) {
        printf("\n");
        for (int y = 0; y < playgroundWidth; ++y) {
            printf("%c", playground[x][y]);
        }
    }
    printf("\n");
}

void updateSnake() {
    for (int i = 0; i < sizeof(snakeCords) / sizeof(snakeCords[0]); ++i) {
        snakeCords[i][1]++;
    }
    for (int i = 0; i < sizeof(snakeCords) / sizeof(snakeCords[0]); ++i) {
        checkForSnakeWrap(i);
        //checkForAppleForSnake();
    }

}

bool checkForSnakeCoordinates(int x, int y) {
    bool returnValue = false;
    for (int i = 0; i < sizeof(snakeCords) / sizeof(snakeCords[0]); ++i) {
        if (x == snakeCords[i][0] && y == snakeCords[i][1]) {
            returnValue = true;
        }
    }
    return returnValue;
}

char getCorrectCharacter(int x, int y) {
    if (checkForPlaygroundBorder(x, y) != 'f') {
        return '*';
    } else if (checkForSnakeCoordinates(x, y)) {
        return 'O';
    } else if (checkForAppleForPlayground(x, y)) {
        return 'A';
    } else {
        return ' ';
    }
}

void updatePlayground() {
    for (int x = 0; x < playgroundHeight; ++x) {
        for (int y = 0; y < playgroundWidth; ++y) {
            playground[x][y] = getCorrectCharacter(x, y);
        }
    }
}

int main() {
    updatePlayground();
    printPlayground();

    unsigned long currentTime = time(NULL) + delay;

    while (true) {
        if (currentTime == time(NULL)) {
            updateSnake();
            updatePlayground();
            printPlayground();
            currentTime = time(NULL) + delay;
        }
    }

}