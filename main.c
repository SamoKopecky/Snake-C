#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>

#define READ_FILE "prekazky.txt"
#define WRITE_FILE "rebricek_hracov.txt"

char playground[25][50];
int playgroundHeight = sizeof(playground) / sizeof(playground[0]);
int playgroundWidth = sizeof(playground[0]) / sizeof(playground[0][0]);
int snakeCords[9][2] = {{2,  2},
                        {3,  2},
                        {4,  2},
                        {5,  2},
                        {6,  2},
                        {7,  2},
                        {8,  2},
                        {9,  2},
                        {10, 2}};
int snakeLength = sizeof(snakeCords) / sizeof(snakeCords[0]);
int appleCords[1][2] = {{8, 10}};
int delay = 1;
char inputKey;
char direction;
bool gameOver = false;
int obstacleArray[5][2];
int obstacleArrayLength = sizeof(obstacleArray) / sizeof(obstacleArray[0]);
int score = 5;
char playerName[10];

bool isSnakeBody(int x, int y) {
    for (int i = 0; i < snakeLength - 2; ++i) {
        if (snakeCords[i][0] == x && snakeCords[i][1] == y) {
            return true;
        }
    }
    return false;
}

bool isObstacle(int x, int y) {
    for (int j = 0; j < obstacleArrayLength - 1; ++j) {
        if (obstacleArray[j][0] == x && obstacleArray[j][1] == y) {
            return true;
        }
    }
    return false;
}

bool isApple(int x, int y) {
    for (int i = 0; i < sizeof(appleCords) / sizeof(appleCords[0]); ++i) {
        if (appleCords[i][0] == x && appleCords[i][1] == y) {
            return true;
        }
    }
    return false;
}

bool isSnake(int x, int y) {
    for (int i = 0; i < snakeLength; ++i) {
        if (x == snakeCords[i][0] && y == snakeCords[i][1]) {
            return true;
        }
    }
    return false;
}

bool isBorder(int x, int y) {
    if (x == 0 || x == playgroundWidth - 1 || y == 0 || y == playgroundHeight - 1) {
        return true;
    }
    return false;
}

void importObstacles() {
    FILE *file;
    int i = 0;

    file = fopen(READ_FILE, "r");

    for (int j = 0; j < obstacleArrayLength; ++j) {
        for (int k = 0; k < 2; ++k) {
            fscanf(file, "%d", &i);
            obstacleArray[j][k] = i;
        }
    }
    fclose(file);
}

void changeDirection() {
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

void printPlayground() {
    for (int y = 0; y < playgroundHeight; ++y) {
        printf("\n");
        for (int x = 0; x < playgroundWidth; ++x) {
            printf("%c", playground[y][x]);
        }
    }
    printf("\n");
}

void updateSnake() {
    changeDirection();

    if (direction) {
        for (int j = 0; j <= snakeLength - 2; ++j) {
            snakeCords[j][0] = snakeCords[j + 1][0];
            snakeCords[j][1] = snakeCords[j + 1][1];
        }
    }

    if (direction == 'w') {
        snakeCords[snakeLength - 1][1]--;
    }
    if (direction == 's') {
        snakeCords[snakeLength - 1][1]++;
    }
    if (direction == 'd') {
        snakeCords[snakeLength - 1][0]++;
    }
    if (direction == 'a') {
        snakeCords[snakeLength - 1][0]--;
    }

    if (isBorder(snakeCords[snakeLength - 1][0], snakeCords[snakeLength - 1][1]) == true ||
        isObstacle(snakeCords[snakeLength - 1][0], snakeCords[snakeLength - 1][1]) == true ||
        isSnakeBody(snakeCords[snakeLength - 1][0], snakeCords[snakeLength - 1][1]) == true) {
        gameOver = true;
    }

}

char getCorrectCharacter(int x, int y) {
    if (isBorder(x, y)) {
        return '*';
    } else if (isObstacle(x, y)) {
        return 'X';
    } else if (isApple(x, y)) {
        return 'A';
    } else if (isSnake(x, y)) {
        return 'O';
    } else {
        return ' ';
    }
}

void updatePlayground() {
    for (int y = 0; y < playgroundHeight; ++y) {
        for (int x = 0; x < playgroundWidth; ++x) {
            playground[y][x] = getCorrectCharacter(x, y);
        }
    }
}

void getPlayerName() {
    printf("Zadaj svoje meno : ");
    scanf("%s", playerName);
    while (getchar() != '\n');
    system("cls");
}

void writeToFile() {
    FILE *file;
    file = fopen(WRITE_FILE, "a");
    fputs(playerName, file);
    fputs(" dosiahol skore : ", file);
    char str[4];
    sprintf(str, "%d", score);
    fputs(str, file);
    fprintf(file, "\n");

}

int main() {
    getPlayerName();
    writeToFile();
    importObstacles();
    updatePlayground();
    printPlayground();
    unsigned long currentTime = time(NULL) + delay;

    while (true) {
        if (kbhit()) {
            inputKey = getch();
        }
        if (currentTime == time(NULL)) {
            system("cls");
            updateSnake();
            updatePlayground();
            printPlayground();
            currentTime = time(NULL) + delay;
        }
        if (gameOver == true) {
            system("cls");
            printf("GAME OVER");
            break;
        }
    }
    getch();
}