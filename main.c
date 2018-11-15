#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include <afxres.h>

#define OBSTACLE 176
#define APPLE 254
#define SNAKE 248
#define HORIZONTAL_LINE 205
#define VERTICAL_LINE 186
#define UPPER_LEFT_CORNER 187
#define LOWER_LEFT_CORNER 188
#define UPPER_RIGHT_CORNER 201
#define LOWER_RIGHT_CORNER 200

#define SNAKE_LENGTH_ON_START 5
#define READ_FILE "prekazky.txt"
#define WRITE_FILE "rebricek_hracov.txt"

char playground[25][50];
int playgroundHeight = sizeof(playground) / sizeof(playground[0]);
int playgroundWidth = sizeof(playground[0]) / sizeof(playground[0][0]);

int appleCords[8][2];
int appleCordsLength = sizeof(appleCords) / sizeof(appleCords[0]);
struct twoDArray {
    int count;
    int **rows;
} snakeCords;

char inputKey;
char direction;

bool gameOver = false;

int obstacleArray[52][2];
int obstacleArrayLength = sizeof(obstacleArray) / sizeof(obstacleArray[0]);

int score = 0;
char playerName[10];

struct players {
    char name[10];
    int score;
} currentPlayer, arrayOfTop10Players[10];

void randomApples() {
    srand(1000 * (unsigned) time(NULL));
    for (int i = 0; i < appleCordsLength; ++i) {
        appleCords[i][0] = rand() % 49 + 1;
        appleCords[i][1] = rand() % 24 + 1;
    }
}

bool isApple(int x, int y) {
    for (int i = 0; i < appleCordsLength; ++i) {
        if (appleCords[i][0] == x && appleCords[i][1] == y) {
            return true;
        }
    }
    return false;
}

void firstAlloc() {
    int i;
    snakeCords.count = SNAKE_LENGTH_ON_START;
    snakeCords.rows = (int **) malloc(5 * sizeof(int) * 2);
    for (i = 0; i < snakeCords.count; ++i) {
        snakeCords.rows[i] = (int *) malloc(2 * sizeof(int));
    }
    for (i = 0; i < snakeCords.count; ++i) {
        snakeCords.rows[i][0] = i + 2;
        snakeCords.rows[i][1] = 2;
    }
}

bool isSnakeBody(int x, int y) {
    for (int i = 0; i < snakeCords.count - 2; ++i) {
        if (snakeCords.rows[i][0] == x && snakeCords.rows[i][1] == y) {
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
    for (int i = 0; i < snakeCords.count; ++i) {
        if (x == snakeCords.rows[i][0] && y == snakeCords.rows[i][1]) {
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
    file = fopen(READ_FILE, "r");
    for (int i = 0; i < obstacleArrayLength; ++i) {
        fscanf(file, "%d,%d", &obstacleArray[i][0], &obstacleArray[i][1]);
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
    printf("\nSCORE : %d", score);
}

void updateSnake() {
    changeDirection();

    for (int i = 0; i <= snakeCords.count - 2; ++i) {
        snakeCords.rows[i][0] = snakeCords.rows[i + 1][0];
        snakeCords.rows[i][1] = snakeCords.rows[i + 1][1];
    }

    if (direction == 'w') {
        snakeCords.rows[snakeCords.count - 1][1]--;
    }
    if (direction == 's') {
        snakeCords.rows[snakeCords.count - 1][1]++;
    }
    if (direction == 'd') {
        snakeCords.rows[snakeCords.count - 1][0]++;
    }
    if (direction == 'a') {
        snakeCords.rows[snakeCords.count - 1][0]--;
    }

    if (isBorder(snakeCords.rows[snakeCords.count - 1][0], snakeCords.rows[snakeCords.count - 1][1]) == true ||
        isObstacle(snakeCords.rows[snakeCords.count - 1][0], snakeCords.rows[snakeCords.count - 1][1]) == true ||
        isSnakeBody(snakeCords.rows[snakeCords.count - 1][0], snakeCords.rows[snakeCords.count - 1][1]) == true) {
        gameOver = true;
    }

}

char getCorrectCharacter(int x, int y) {
    if (isBorder(x, y)) {
        return '*';
    } else if (isApple(x, y)) {
        return APPLE;
    } else if (isObstacle(x, y)) {
        return OBSTACLE;
    } else if (isSnake(x, y)) {
        return SNAKE;
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

void addCurrentPlayerToTop10(int indexOfPlayer) {
    int i;
    for (i = 9; i >= indexOfPlayer; --i) {
        arrayOfTop10Players[i + 1] = arrayOfTop10Players[i];
    }
    arrayOfTop10Players[indexOfPlayer].score = currentPlayer.score;
    for (i = 0; i < 10; ++i) {
        arrayOfTop10Players[indexOfPlayer].name[i] = currentPlayer.name[i];
    }
}

void updateTop10List() {
    int i;
    currentPlayer.score = score;
    for (i = 0; i < 10; ++i) {
        currentPlayer.name[i] = playerName[i];
    }
    FILE *file;
    file = fopen(WRITE_FILE, "r");
    for (i = 0; i < 10; ++i) {
        fscanf(file, "%s ", arrayOfTop10Players[i].name);
        fseek(file, 17, SEEK_CUR);
        fscanf(file, "%d", &arrayOfTop10Players[i].score);
    }
    for (i = 0; i < 10; ++i) {
        if (arrayOfTop10Players[i].score < currentPlayer.score) {
            addCurrentPlayerToTop10(i);
            break;
        }
    }
    fclose(file);
    file = fopen(WRITE_FILE, "w");
    for (i = 0; i < 10; ++i) {
        fprintf(file, "%s dosiahol skore : %d\n", arrayOfTop10Players[i].name, arrayOfTop10Players[i].score);
    }
    fclose(file);
}

void writeOutTop10() {
    printf("\n");
    for (int i = 0; i < 10; ++i) {
        printf("%s dosiahol skore : %d\n", arrayOfTop10Players[i].name, arrayOfTop10Players[i].score);
    }
}

void printInstructions() {
    printf("Pohyb : W -> hore, S -> dole, D -> doprava, A -> dolava.\n");
    printf("Ak zjes jablko pripocita sa ti 1 k tvojmu skore.\n");
    printf("Kolizia zo stenou hracej plochy, z prekazkami a telom hada ukoncia hru a ulozia skore.\n");
    printf("Had -> %c\n", SNAKE);
    printf("Jablko -> %c\n", APPLE);
    printf("Stena hracej plochy -> %c\n", OBSTACLE);
    printf("Po zadani tvojho mena sa hra zacne !\n");
}

void eatApple() {
    int i;
    if (isApple(snakeCords.rows[snakeCords.count - 1][0], snakeCords.rows[snakeCords.count - 1][1])) {
        int **temp;
        temp = (int **) malloc(snakeCords.count * sizeof(int) * 2);
        for (i = 0; i < snakeCords.count; ++i) {
            temp[i] = (int *) malloc(2 * sizeof(int));
        }
        for (i = 0; i < snakeCords.count; ++i) {
            temp[i][0] = snakeCords.rows[i][0];
            temp[i][1] = snakeCords.rows[i][1];
        }
        free(snakeCords.rows);
        score++;
        snakeCords.count++;
        snakeCords.rows = (int **) malloc(snakeCords.count * sizeof(int) * 2);
        for (i = 0; i < snakeCords.count; ++i) {
            snakeCords.rows[i] = (int *) malloc(2 * sizeof(int));
        }

        for (i = 1; i < snakeCords.count; ++i) {
            snakeCords.rows[i][0] = temp[i - 1][0];
            snakeCords.rows[i][1] = temp[i - 1][1];
        }

        if (snakeCords.rows[2][0] - 1 == snakeCords.rows[1][0]) {
            snakeCords.rows[0][0] = snakeCords.rows[1][0] - 1;
            snakeCords.rows[0][1] = snakeCords.rows[1][1];
        } else if (snakeCords.rows[2][0] + 1 == snakeCords.rows[1][0]) {
            snakeCords.rows[0][0] = snakeCords.rows[1][0] + 1;
            snakeCords.rows[0][1] = snakeCords.rows[1][1];
        } else if (snakeCords.rows[2][1] - 1 == snakeCords.rows[1][1]) {
            snakeCords.rows[0][0] = snakeCords.rows[1][0];
            snakeCords.rows[0][1] = snakeCords.rows[1][1] - 1;
        } else if (snakeCords.rows[2][1] + 1 == snakeCords.rows[1][1]) {
            snakeCords.rows[0][0] = snakeCords.rows[1][0];
            snakeCords.rows[0][1] = snakeCords.rows[1][1] + 1;
        }

        srand(1000 * (unsigned) time(NULL));
        int index = 2;

        for (i = 0; i < 8; ++i) {
            if (snakeCords.rows[snakeCords.count - 1][0] == appleCords[i][0] &&
                snakeCords.rows[snakeCords.count - 1][1] == appleCords[i][1]) {
                index = i;
                break;
            }
        }
        appleCords[index][0] = rand() % 49 + 1;
        appleCords[index][1] = rand() % 24 + 1;


        updateSnake();
    }
}

int main() {
    printInstructions();
    direction = 'd';
    firstAlloc();
    getPlayerName();
    randomApples();
    importObstacles();
    updatePlayground();
    printPlayground();

    struct timespec tim;
    tim.tv_sec = 0;
    tim.tv_nsec = 500000000;
    while (true) {
        if (kbhit()) {
            inputKey = getch();
        }
        nanosleep(&tim, NULL);
        system("CLS");
        updateSnake();
        if (gameOver) {
            break;
        }
        eatApple();
        updatePlayground();
        printPlayground();

    }
    system("cls");
    char line[11];
    for (int i = 0; i < 11; ++i) {
        line[i] = HORIZONTAL_LINE;
    }
    printf("%c%s%c", UPPER_RIGHT_CORNER, line, UPPER_LEFT_CORNER);
    printf("\n%c GAME OVER %c\n", VERTICAL_LINE, VERTICAL_LINE);
    printf("%c%s%c", LOWER_RIGHT_CORNER, line, LOWER_LEFT_CORNER);
    updateTop10List();
    writeOutTop10();
    getch();
}

/*
 * TODO: jablka abi sa nezobrazovali na prekazkach ani na snakovi
 * TODO: zmenit velkost pola prekazok polda poctu prekazok
 * TODO: lepsiu mriezku
 */