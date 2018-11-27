#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "initializationOfGame.h"

#define SNAKE_LENGTH_ON_START (char) 5
#define OBSTACLE (char) 176
#define APPLE (char) 254
#define SNAKE (char) 248
#define HORIZONTAL_LINE (char) 205
#define VERTICAL_LINE (char) 186
#define UPPER_RIGHT_CORNER (char) 187
#define LOWER_RIGHT_CORNER (char) 188
#define UPPER_LEFT_CORNER (char) 201
#define LOWER_LEFT_CORNER (char) 200


char playerName[10];
int diff;
int playgroundWidth;
int playgroundHeight;
char playground[25][50];
int score;
struct twoDArray snakeCords;
int appleCordsLength;
int appleCords[8][2];

void
getValuesInitializationOfGame(char ptrPlayerName[10], int *ptrDiff, int *ptrPlaygroundHeight, int *ptrPlaygroundWidth,
                              char ptrPlayground[25][50], int *ptrScore, struct twoDArray *ptrSnakeCords,
                              int ptrAppleCords[8][2], int *ptrAppleCordsLength) {
    for (int i = 0; i < 10; ++i) {
        playerName[i] = ptrPlayerName[i];
    }
    diff = *ptrDiff;
    playgroundHeight = *ptrPlaygroundHeight;
    playgroundWidth = *ptrPlaygroundWidth;
    for (int j = 0; j < 25; ++j) {
        for (int i = 0; i < 50; ++i) {
            playground[j][i] = ptrPlayground[j][i];
        }
    }
    score = *ptrScore;
    snakeCords = *ptrSnakeCords;
    for (int k = 0; k < 8; ++k) {
        for (int i = 0; i < 2; ++i) {
            appleCords[k][i] = ptrAppleCords[k][i];
        }
    }
    appleCordsLength = *ptrAppleCordsLength;
}

// Nacitanie mena hraca
void getPlayerName() {
    printf("Zadaj svoje meno : ");
    scanf("%s", playerName);
    while (getchar() != '\n');
}

// Vytlacenie instrukcii pre hru
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

// Nacitanie obtiaznosti hry
void getDifficulty() {
    printf("Zadaj obtiaznost : ");
    scanf("%d", &diff);
    while (getchar() != '\n');
    system("cls");
}

// Vybranie spravneho znaku pre prvotne tlacenie
char getCorrectCharacter(int x, int y) {
    if (isBorder(x, y)) {
        return getCorrectBorder(x, y);
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

// Vrati znak podla toho ake je x a y
char getCorrectBorder(int x, int y) {
    if (x == 0 && y == 0) {
        return UPPER_LEFT_CORNER;
    } else if (x == 0 && y == playgroundHeight - 1) {
        return LOWER_LEFT_CORNER;
    } else if (x == playgroundWidth - 1 && y == 0) {
        return UPPER_RIGHT_CORNER;
    } else if (x == playgroundWidth - 1 && y == playgroundHeight - 1) {
        return LOWER_RIGHT_CORNER;
    } else if (x == 0 || x == playgroundWidth - 1) {
        return VERTICAL_LINE;
    } else if (y == 0 || y == playgroundHeight - 1) {
        return HORIZONTAL_LINE;
    }
}

// Prvotne tlacenie hracej plochy
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

// Prva alokacia pamete pola pre suradnice hada
void firstAlloc() {
    int i;
    snakeCords.count = SNAKE_LENGTH_ON_START;
    snakeCords.rows = (int **) malloc(SNAKE_LENGTH_ON_START * sizeof(int) * 2);
    for (i = 0; i < snakeCords.count; ++i) {
        snakeCords.rows[i] = (int *) malloc(2 * sizeof(int));
    }

    // Prvotna poloha hada
    for (i = 0; i < snakeCords.count; ++i) {
        snakeCords.rows[i][0] = i + 2;
        snakeCords.rows[i][1] = 2;
    }
}

// Importovanie jablk do pola suradnic jablk
void importApples() {
    srand(1000 * (unsigned) time(NULL));
    for (int i = 0; i < appleCordsLength; ++i) {
        appleCords[i][0] = rand() % 48 + 1;
        appleCords[i][1] = rand() % 23 + 1;
        // Kontrola ak su suradnice jablka take iste ako suradnice prekazky alebo hada a ak ano tak odcita od i
        if (isObstacle(appleCords[i][0], appleCords[i][1]) || isSnake(appleCords[i][0], appleCords[i][1])) {
            i--;
        }
    }
}