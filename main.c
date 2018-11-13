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
int appleCords[8][2];
int appleCordsLenght = sizeof(appleCords) / sizeof(appleCords[0]);
struct twoDArray {
    int count;
    int **rows;
} snakeCords2D;
char inputKey;
char direction;
bool gameOver = false;
int obstacleArray[5][2];
int obstacleArrayLength = sizeof(obstacleArray) / sizeof(obstacleArray[0]);
int score = 0;
char playerName[10];
struct highScores {
    char meno[10];
    int score;
} currentPlayer;
struct highScores arrayOfScores[10];

void randomApples() {
    srand(1000 * (unsigned) time(NULL));
    for (int i = 0; i < appleCordsLenght; ++i) {
        appleCords[i][0] = rand() % 49 + 1;
        appleCords[i][1] = rand() % 24 + 1;
    }
}

bool isApple(int x, int y) {
    for (int i = 0; i < sizeof(appleCords) / sizeof(appleCords[0]); ++i) {
        if (appleCords[i][0] == x && appleCords[i][1] == y) {
            return true;
        }
    }
    return false;
}

void firstAlloc() {
    snakeCords2D.count = 5;
    snakeCords2D.rows = (int **) malloc(5 * sizeof(int) * 2);
    for (int i = 0; i < snakeCords2D.count; ++i) {
        snakeCords2D.rows[i] = (int *) malloc(2 * sizeof(int));
    }
    for (int j = 0; j < snakeCords2D.count; ++j) {
        snakeCords2D.rows[j][0] = j + 2;
        snakeCords2D.rows[j][1] = 2;
    }
}

bool isSnakeBody(int x, int y) {
    for (int i = 0; i < snakeCords2D.count - 2; ++i) {
        if (snakeCords2D.rows[i][0] == x && snakeCords2D.rows[i][1] == y) {
            return true;
        }
    }
    return false;
}

bool isObstacle(int x, int y) {
    for (int j = 0; j < obstacleArrayLength; ++j) {
        if (obstacleArray[j][0] == x && obstacleArray[j][1] == y) {
            return true;
        }
    }
    return false;
}

bool isSnake(int x, int y) {
    for (int i = 0; i < snakeCords2D.count; ++i) {
        if (x == snakeCords2D.rows[i][0] && y == snakeCords2D.rows[i][1]) {
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
        fscanf(file, "%d,%d", &obstacleArray[j][0], &obstacleArray[j][1]);
    }
    fclose(file);
}

void changeDirection() {
    if (!((inputKey == 'w' && direction == 's') ||
        (inputKey == 's' && direction == 'w') ||
        (inputKey == 'd' && direction == 'a') ||
        (inputKey == 'a' && direction == 'd'))) {
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

void printPlayground() {
    for (int y = 0; y < playgroundHeight; ++y) {
        printf("\n");
        for (int x = 0; x < playgroundWidth; ++x) {
            printf("%c", playground[y][x]);
        }
    }
    printf("\nTvoje skore je : %d", score);
}

void updateSnake() {
    changeDirection();

    if (direction) {
        for (int j = 0; j <= snakeCords2D.count - 2; ++j) {
            snakeCords2D.rows[j][0] = snakeCords2D.rows[j + 1][0];
            snakeCords2D.rows[j][1] = snakeCords2D.rows[j + 1][1];
        }
    }

    if (direction == 'w') {
        snakeCords2D.rows[snakeCords2D.count - 1][1]--;
    }
    if (direction == 's') {
        snakeCords2D.rows[snakeCords2D.count - 1][1]++;
    }
    if (direction == 'd') {
        snakeCords2D.rows[snakeCords2D.count - 1][0]++;
    }
    if (direction == 'a') {
        snakeCords2D.rows[snakeCords2D.count - 1][0]--;
    }

    if (isBorder(snakeCords2D.rows[snakeCords2D.count - 1][0], snakeCords2D.rows[snakeCords2D.count - 1][1]) == true ||
        isObstacle(snakeCords2D.rows[snakeCords2D.count - 1][0], snakeCords2D.rows[snakeCords2D.count - 1][1]) ==
        true ||
        isSnakeBody(snakeCords2D.rows[snakeCords2D.count - 1][0], snakeCords2D.rows[snakeCords2D.count - 1][1]) ==
        true) {
        gameOver = true;
    }

}

char getCorrectCharacter(int x, int y) {
    if (isBorder(x, y)) {
        return '*';
    } else if (isApple(x, y)) {
        return 'A';
    } else if (isObstacle(x, y)) {
        return 'X';
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

void addCurrentPlayerToTop10(int i) {
    for (int j = 9; j >= i; --j) {
        arrayOfScores[j + 1] = arrayOfScores[j];

    }
    arrayOfScores[i].score = currentPlayer.score;
    for (int k = 0; k < 10; ++k) {
        arrayOfScores[i].meno[k] = currentPlayer.meno[k];
    }
}

void writeToFile() {
    currentPlayer.score = score;
    for (int k = 0; k < 10; ++k) {
        currentPlayer.meno[k] = playerName[k];
    }
    FILE *file;
    file = fopen(WRITE_FILE, "r");
    for (int i = 0; i < 10; ++i) {
        fscanf(file, "%s ", arrayOfScores[i].meno);
        fseek(file, 17, SEEK_CUR);
        fscanf(file, "%d", &arrayOfScores[i].score);
    }
    for (int k = 0; k < 10; ++k) {
        if (arrayOfScores[k].score < currentPlayer.score) {
            addCurrentPlayerToTop10(k);
            break;
        }
    }
    fclose(file);
    file = fopen(WRITE_FILE, "w");
    for (int j = 0; j < 10; ++j) {
        fprintf(file, "%s dosiahol skore : %d\n", arrayOfScores[j].meno, arrayOfScores[j].score);
    }
    fclose(file);
}

void writeOutTop10() {
    printf("\n");
    for (int i = 0; i < 10; ++i) {
        printf("%s dosiahol skore : %d\n", arrayOfScores[i].meno, arrayOfScores[i].score);
    }
}

void changeSize() {
    if (isApple(snakeCords2D.rows[snakeCords2D.count - 1][0], snakeCords2D.rows[snakeCords2D.count - 1][1])) {
        int **temp;
        temp = (int **) malloc(snakeCords2D.count * sizeof(int) * 2);
        for (int j = 0; j < snakeCords2D.count; ++j) {
            temp[j] = (int *) malloc(2 * sizeof(int));
        }
        for (int m = 0; m < snakeCords2D.count; ++m) {
            temp[m][0] = snakeCords2D.rows[m][0];
            temp[m][1] = snakeCords2D.rows[m][1];
        }
        free(snakeCords2D.rows);
        score++;
        snakeCords2D.count++;
        snakeCords2D.rows = (int **) malloc(snakeCords2D.count * sizeof(int) * 2);
        for (int i = 0; i < snakeCords2D.count; ++i) {
            snakeCords2D.rows[i] = (int *) malloc(2 * sizeof(int));
        }

        for (int k = 1; k < snakeCords2D.count; ++k) {
            snakeCords2D.rows[k][0] = temp[k - 1][0];
            snakeCords2D.rows[k][1] = temp[k - 1][1];
        }

        if (snakeCords2D.rows[2][0] - 1 == snakeCords2D.rows[1][0]) {
            snakeCords2D.rows[0][0] = snakeCords2D.rows[1][0] - 1;
            snakeCords2D.rows[0][1] = snakeCords2D.rows[1][1];
        }
        srand(1000 * (unsigned) time(NULL));
        int index = 2;

        for (int l = 0; l < 8; ++l) {
            if (snakeCords2D.rows[snakeCords2D.count - 1][0] == appleCords[l][0] &&
                snakeCords2D.rows[snakeCords2D.count - 1][1] == appleCords[l][1]) {
                index = l;
                break;
            }
        }

        appleCords[index][0] = rand() % 49 + 1;
        appleCords[index][1] = rand() % 24 + 1;


        updateSnake();
    }
}

int main() {
    direction = 'd';
    firstAlloc();
    getPlayerName();
    randomApples();
    importObstacles();
    updatePlayground();
    printPlayground();

    struct timespec tim;
    tim.tv_sec = 0;
    tim.tv_nsec = 250000000;
    while (!gameOver) {
        if (kbhit()) {
            inputKey = getch();
        }
        nanosleep(&tim, NULL);
        system("cls");
        updateSnake();
        changeSize();
        updatePlayground();
        printPlayground();

    }
    system("cls");
    printf("GAME OVER");
    writeToFile();
    writeOutTop10();
    getch();
}