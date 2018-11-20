#include <stdio.h>
#include <conio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include <afxres.h>

#define READ_FILE "prekazky.txt"
#define WRITE_FILE "rebricek_hracov.txt"

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

HANDLE hStdout;


struct twoDArray {
    int count;
    int **rows;
};

struct player {
    char name[10];
    int score;
};

char playground[25][50];
int playgroundHeight = sizeof(playground) / sizeof(playground[0]);
int playgroundWidth = sizeof(playground[0]) / sizeof(playground[0][0]);
int obstacleArray[52][2];
int obstacleArrayLength = sizeof(obstacleArray) / sizeof(obstacleArray[0]);
int appleCords[8][2];
int appleCordsLength = sizeof(appleCords) / sizeof(appleCords[0]);
struct twoDArray snakeCords;
char inputKey;
char direction;
bool gameOver = false;
int score = 0;
char playerName[10];
int diff;
COORD cords;
char c[2];
struct player currentPlayer;
struct player arrayOfTop10player[10];
unsigned long charsWritten;
int oldSnakeCords[2];

bool isApple(int x, int y) {
    for (int i = 0; i < appleCordsLength; ++i) {
        if (appleCords[i][0] == x && appleCords[i][1] == y) {
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

bool isSnake(int x, int y) {
    for (int i = 0; i < snakeCords.count; ++i) {
        if (x == snakeCords.rows[i][0] && y == snakeCords.rows[i][1]) {
            return true;
        }
    }
    return false;
}

bool isObstacle(int x, int y) {
    for (int i = 0; i < obstacleArrayLength; ++i) {
        if (obstacleArray[i][0] == x && obstacleArray[i][1] == y) {
            return true;
        }
    }
    return false;
}

void getPlayerName() {
    printf("Zadaj svoje meno : ");
    scanf("%s", playerName);
    while (getchar() != '\n');
}

void printInstructions() {
    printf("Pohyb : W -> hore, S -> dole, D -> doprava, A -> dolava.\nAk zjes jablko pripocita sa ti 1 k tvojmu skore.\n");
    printf("Kolizia zo stenou hracej plochy, z prekazkami a telom hada ukoncia hru a ulozia skore.\n");
    printf("Had -> %c\n", SNAKE);
    printf("Jablko -> %c\n", APPLE);
    printf("Stena hracej plochy -> %c\n", OBSTACLE);
    printf("Obtiaznosti hry : \n");
    printf("1 - easy\n");
    printf("2 - medium\n");
    printf("3 - hard\n");
    printf("Po zadani tvojho mena a optiaznosti sa hra zacne !\n");
}

void getDifficulty() {
    printf("Zadaj obtiaznost : ");
    scanf("%d", &diff);
    while (getchar() != '\n');
    system("cls");
}

char getCorrectCharacter(int x, int y) {
    if (isBorder(x, y)) {
        return '*';
    } else if (isApple(x, y)) {
        return (char)APPLE;
    } else if (isObstacle(x, y)) {
        return (char)OBSTACLE;
    } else if (isSnake(x, y)) {
        return (char)SNAKE;
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
    for (int y = 0; y < playgroundHeight; ++y) {
        if (y != 0) {
            printf("\n");
        }
        for (int x = 0; x < playgroundWidth; ++x) {
            printf("%c", playground[y][x]);
        }
    }
    printf("\nSCORE : %d", score);
}

void addCurrentPlayerToTop10(int indexOfPlayer) {
    int i;
    for (i = 9; i >= indexOfPlayer; --i) {
        arrayOfTop10player[i + 1] = arrayOfTop10player[i];
    }
    arrayOfTop10player[indexOfPlayer].score = currentPlayer.score;
    for (i = 0; i < 10; ++i) {
        arrayOfTop10player[indexOfPlayer].name[i] = currentPlayer.name[i];
    }
}

void importObstacles() {
    FILE *file;
    file = fopen(READ_FILE, "r");
    for (int i = 0; i < obstacleArrayLength; ++i) {
        fscanf(file, "%d,%d", &obstacleArray[i][0], &obstacleArray[i][1]);
    }
    fclose(file);
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
        fscanf(file, "%s ", arrayOfTop10player[i].name);
        fseek(file, 17, SEEK_CUR);
        fscanf(file, "%d", &arrayOfTop10player[i].score);
    }
    for (i = 0; i < 10; ++i) {
        if (arrayOfTop10player[i].score < currentPlayer.score) {
            addCurrentPlayerToTop10(i);
            break;
        }
    }
    fclose(file);
    file = fopen(WRITE_FILE, "w");
    for (i = 0; i < 10; ++i) {
        fprintf(file, "%s dosiahol skore : %d\n", arrayOfTop10player[i].name, arrayOfTop10player[i].score);
    }
    fclose(file);
}

void firstAlloc() {
    int i;
    snakeCords.count = SNAKE_LENGTH_ON_START;
    snakeCords.rows = (int **) malloc(SNAKE_LENGTH_ON_START * sizeof(int) * 2);
    for (i = 0; i < snakeCords.count; ++i) {
        snakeCords.rows[i] = (int *) malloc(2 * sizeof(int));
    }
    for (i = 0; i < snakeCords.count; ++i) {
        snakeCords.rows[i][0] = i + 2;
        snakeCords.rows[i][1] = 2;
    }
}

void importApples() {
    srand(1000 * (unsigned) time(NULL));
    for (int i = 0; i < appleCordsLength; ++i) {
        appleCords[i][0] = rand() % 48 + 1;
        appleCords[i][1] = rand() % 23 + 1;
        if (isObstacle(appleCords[i][0], appleCords[i][1]) || isSnake(appleCords[i][0], appleCords[i][1])) {
            i--;
        }
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

void printSnake() {
    cords.X = (short) snakeCords.rows[snakeCords.count - 1][0];
    cords.Y = (short) snakeCords.rows[snakeCords.count - 1][1];
    c[0] = SNAKE;
    c[1] = '\0';
    WriteConsoleOutputCharacter(hStdout, c, 1, cords, &charsWritten);
    cords.X = (short) oldSnakeCords[0];
    cords.Y = (short) oldSnakeCords[1];
    WriteConsoleOutputCharacter(hStdout, " ", 1, cords, &charsWritten);
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

void updateSnake() {
    oldSnakeCords[0] = snakeCords.rows[0][0];
    oldSnakeCords[1] = snakeCords.rows[0][1];
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

void updateScore() {
    cords.X = 8;
    cords.Y = 25;
    sprintf(c, "%d", score);
    WriteConsoleOutputCharacter(hStdout, c, 1, cords, &charsWritten);
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
        int index = 0;

        for (i = 0; i < 8; ++i) {
            if (snakeCords.rows[snakeCords.count - 1][0] == appleCords[i][0] &&
                snakeCords.rows[snakeCords.count - 1][1] == appleCords[i][1]) {
                index = i;
                break;
            }
        }
        do {
            appleCords[index][0] = rand() % 48 + 1;
            appleCords[index][1] = rand() % 23 + 1;
        } while (isObstacle(appleCords[i][0], appleCords[i][1]) ||
                 isSnake(appleCords[i][0], appleCords[i][1]));

        cords.X = (short) appleCords[index][0];
        cords.Y = (short) appleCords[index][1];
        c[0] = (char)APPLE;
        c[1] = '\0';
        WriteConsoleOutputCharacter(hStdout, c, 1, cords, &charsWritten);
        updateScore();
    }
}

void writeOutTop10() {
    printf("\n");
    for (int i = 0; i < 10; ++i) {
        printf("%s dosiahol skore : %d\n", arrayOfTop10player[i].name, arrayOfTop10player[i].score);
    }
}

int main() {
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    printInstructions();
    direction = 'd';
    firstAlloc();
    getPlayerName();
    getDifficulty();
    importObstacles();
    importApples();
    updatePlayground();
    struct timespec tim;
    tim.tv_sec = 0;
    switch (diff) {
        case 1  :
            tim.tv_nsec = 250000000;
            break;
        case 2 :
            tim.tv_nsec = 125000000;
            break;
        case 3 :
            tim.tv_nsec = 100000000;
            break;
        default:
            break;
    }
    while (true) {

        if (kbhit()) {
            inputKey = getch();
        }
        nanosleep(&tim, NULL);
        updateSnake();
        if (gameOver) {
            break;
        }
        eatApple();
        printSnake();
    }
    system("cls");
    char line[11];
    for (int i = 0; i < 11; ++i) {
        line[i] = (char)HORIZONTAL_LINE;
    }
    printf("%c%s%c", UPPER_RIGHT_CORNER, line, UPPER_LEFT_CORNER);
    printf("\n%c GAME OVER %c\n", VERTICAL_LINE, VERTICAL_LINE);
    printf("%c%s%c", LOWER_RIGHT_CORNER, line, LOWER_LEFT_CORNER);
    updateTop10List();
    writeOutTop10();
    getch();
}