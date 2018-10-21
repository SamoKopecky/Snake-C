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


void checkSnakeWrap() {
    for (int i = 0; i < sizeof(snakeCords) / sizeof(snakeCords[0]); ++i) {
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
    checkSnakeWrap();
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