#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>

#define READ_FILE "prekazky.txt"

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
char inputKey;
char direction;
bool gameOver = false;
int obstacleArray[10][2];

bool checkForObstacles(int x, int y) {
    for (int j = 0; j < 10; ++j) {
        if (obstacleArray[j][1] == x && obstacleArray[j][0] == y) {
            return true;
        }
    }
    return false;
}

void importObstacles() {
    FILE *file;
    int i = 0;

    file = fopen(READ_FILE, "r");

    for (int j = 0; j < 10; ++j) {
        for (int k = 0; k < 2; ++k) {
            fscanf(file, "%d", &i);
            obstacleArray[j][k] = i;
        }
    }
    fclose(file);
}

void changeDirection() {
    for (int i = 0; i < sizeof(snakeCords) / sizeof(snakeCords[0]); ++i) {
        if (inputKey == 'w') {
            direction = 'w';
        }
        if (inputKey == 's') {
            direction = 's';
        }
        if (inputKey == 'd') {
            direction = 'd';
        }
        if (inputKey == 'a') {
            direction = 'a';
        }
    }

}

char checkForPlaygroundBorder(int x, int y) {
    if (x == 0 || x == playgroundHeight - 1 || y == 0 || y == playgroundWidth - 1) {
        return 't';
    }
    return 'f';
}

bool checkForAppleForPlayground(int x, int y) {
    for (int i = 0; i < sizeof(appleCords) / sizeof(appleCords[0]); ++i) {
        if (appleCords[i][0] == x && appleCords[i][1] == y) {
            return true;
        }
        return false;
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
    changeDirection();
    //for (int j = 0; j < sizeof(snakeCords) / sizeof(snakeCords[0]); ++j) {
    if (direction == 'w') {
        snakeCords[4][0]--;
    }
    if (direction == 's') {
        snakeCords[4][0]++;
    }
    if (direction == 'd') {
        snakeCords[4][1]++;
    }
    if (direction == 'a') {
        snakeCords[4][1]--;
    }
    //}
    for (int i = 0; i < sizeof(snakeCords) / sizeof(snakeCords[0]); ++i) {
        if (checkForPlaygroundBorder(snakeCords[i][0], snakeCords[i][1]) == 't') {
            gameOver = true;
        }
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
    } else if (checkForObstacles(x, y)) {
        return 'X';
    } else if (checkForAppleForPlayground(x, y)) {
        return 'A';
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
    importObstacles();

    printf("test");
    unsigned long currentTime = time(NULL) + delay;

    while (true) {
        if (gameOver == true) {
            system("cls");
            printf("GAME OVER");
            break;
        }
        if (currentTime == time(NULL)) {
            printf("test");
            system("cls");
            updateSnake();
            updatePlayground();
            printPlayground();
            currentTime = time(NULL) + delay;
            if (kbhit()) {
                inputKey = getch();
            }
        }
    }
    getchar();
}