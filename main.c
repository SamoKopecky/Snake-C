#include <stdio.h>
#include <stdbool.h>
#include <time.h>

char playground[25][50];
int snakeCords[5][2] = {{2, 2},
                        {2, 3},
                        {2, 4},
                        {2, 5},
                        {2, 6}};
int delay = 1;

void printPlayground() {
    for (int x = 0; x < sizeof(playground) / sizeof(playground[0]); ++x) {
        printf("\n");
        for (int y = 0; y < sizeof(playground[0]) / sizeof(playground[0][0]); ++y) {
            printf("%c", playground[x][y]);
        }
    }
    printf("\n");
}

void updateSnake() {
    for (int i = 0; i < sizeof(snakeCords) / sizeof(snakeCords[0]); ++i) {
        snakeCords[i][1]++;
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

char checkForPlaygroundBorder(int x, int y) {
    if (x == 0) {
        return 'l';
    }
    if (x == sizeof(playground) / sizeof(playground[0]) - 1) {
        return 'r';
    }
    if (y == 0) {
        return 'u';
    }
    if (y == sizeof(playground[0]) - 1) {
        return 'd';
    }
    return 'f';
}

void wrapSnake(int x, int y) {
    for (int i = sizeof(snakeCords) / sizeof(snakeCords[0]); i > 0; --i) {
        if (checkForPlaygroundBorder(i, y))
    }
    if (checkForPlaygroundBorder(x, y) == 'r') {
        printf("test");
        snakeCords[i][0] -= snakeCords[0][0];
    }
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
    for (int x = 0; x < sizeof(playground) / sizeof(playground[0]); ++x) {
        for (int y = 0; y < sizeof(playground[0]); ++y) {
            wrapSnake(x, y);
            playground[x][y] = getCorrectCharacter(x, y);
        }
    }
}

int main() {
    //updatePlayground();
    //printPlayground();

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