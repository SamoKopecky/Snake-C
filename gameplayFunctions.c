#include <afxres.h>
#include <time.h>
#include <stdio.h>
#include "gameplayFunctions.h"

#define APPLE (char) 254
#define SNAKE (char) 248

int playgroundWidth;
int playgroundHeight;
struct twoDArray snakeCords;
int appleCordsLength;
int appleCords[8][2];
int obstacleArray[52][2];
int obstacleArrayLength;
int obstacleArray[52][2];
int obstacleArrayLength;
COORD cords;
int oldSnakeCords[2];
unsigned long charsWritten;
int score;
char direction;
bool gameOver;
char inputKey;
char c[2];
HANDLE hStdout;

// Funkcia na ziskanie hodnout z main.c
void
getValuesGameplayFunctions(HANDLE *ptrHStdout, char *ptrInputKey, bool *ptrGameOver, char *ptrDirection, int *ptrScore,
                           int ptrObstacleArray[52][2], const int *ptrObstacleArrayLength, int *ptrPlaygroundHeight,
                           int *ptrPlaygroundWidth, struct twoDArray *ptrSnakeCords, int ptrAppleCords[8][2],
                           int *ptrAppleCordsLength) {
    obstacleArrayLength = *ptrObstacleArrayLength;
    for (int i = 0; i < 52; ++i) {
        for (int j = 0; j < 2; ++j) {
            obstacleArray[i][j] = ptrObstacleArray[i][j];
        }
    }
    playgroundHeight = *ptrPlaygroundHeight;
    playgroundWidth = *ptrPlaygroundWidth;
    snakeCords = *ptrSnakeCords;
    for (int k = 0; k < 8; ++k) {
        for (int i = 0; i < 2; ++i) {
            appleCords[k][i] = ptrAppleCords[k][i];
        }
    }
    appleCordsLength = *ptrAppleCordsLength;
    score = *ptrScore;
    direction = *ptrDirection;
    gameOver = *ptrGameOver;
    inputKey = *ptrInputKey;
    hStdout = *ptrHStdout;
}

// Kontrola jablka na suradniciach x a y
bool isApple(int x, int y) {
    for (int i = 0; i < appleCordsLength; ++i) {
        if (appleCords[i][0] == x && appleCords[i][1] == y) {
            return true;
        }
    }
    return false;
}

// Kontrola okraja hracej plochy na suradniciach x a y
bool isBorder(int x, int y) {
    if (x == 0 || x == playgroundWidth - 1 || y == 0 || y == playgroundHeight - 1) {
        return true;
    }
    return false;
}

// Kontrola hada na suradniciach x a y
bool isSnake(int x, int y) {
    for (int i = 0; i < snakeCords.count; ++i) {
        if (x == snakeCords.rows[i][0] && y == snakeCords.rows[i][1]) {
            return true;
        }
    }
    return false;
}

// Kontrola prekazky na suradniciach x a y
bool isObstacle(int x, int y) {
    for (int i = 0; i < obstacleArrayLength; ++i) {
        if (obstacleArray[i][0] == x && obstacleArray[i][1] == y) {
            return true;
        }
    }
    return false;
}

// Kontrola tela hada na suradniciach x a y
bool isSnakeBody(int x, int y) {
    for (int i = 0; i < snakeCords.count - 2; ++i) {
        if (snakeCords.rows[i][0] == x && snakeCords.rows[i][1] == y) {
            return true;
        }
    }
    return false;
}

// Tlacenie hada v terminali
void printSnake() {
    cords.X = (short) snakeCords.rows[snakeCords.count - 1][0];
    cords.Y = (short) snakeCords.rows[snakeCords.count - 1][1];
    c[0] = SNAKE;
    c[1] = '\0';
    // Vytlaci novu hlavu hada
    WriteConsoleOutputCharacter(hStdout, c, 1, cords, &charsWritten);
    cords.X = (short) oldSnakeCords[0];
    cords.Y = (short) oldSnakeCords[1];
    // Zmaze stary cvhost hada
    WriteConsoleOutputCharacter(hStdout, " ", 1, cords, &charsWritten);
}

// Zmena smeru pohybu hada
void changeDirection() {
    // Zabranie cuvania hada
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

// Funkcie pre aktualizaciu suradnic hada
void updateSnake() {
    // Ulozenie starych suradnic do docasneho pola
    oldSnakeCords[0] = snakeCords.rows[0][0];
    oldSnakeCords[1] = snakeCords.rows[0][1];
    changeDirection();
    // Posunutie suradnic kazdej bunky hada
    for (int i = 0; i <= snakeCords.count - 2; ++i) {
        snakeCords.rows[i][0] = snakeCords.rows[i + 1][0];
        snakeCords.rows[i][1] = snakeCords.rows[i + 1][1];
    }
    // Zmena smeru
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
    // Kontrola ci had narazil do steny hracej plochy alebo do seba alebo do prekazky
    if (isBorder(snakeCords.rows[snakeCords.count - 1][0], snakeCords.rows[snakeCords.count - 1][1]) == true ||
        isObstacle(snakeCords.rows[snakeCords.count - 1][0], snakeCords.rows[snakeCords.count - 1][1]) == true ||
        isSnakeBody(snakeCords.rows[snakeCords.count - 1][0], snakeCords.rows[snakeCords.count - 1][1]) == true) {
        gameOver = true;
    }
}

// Tlacenie skore
void updateScore() {
    cords.X = 8;
    cords.Y = 25;
    sprintf(c, "%d", score);
    WriteConsoleOutputCharacter(hStdout, c, sizeof(c), cords, &charsWritten);
}

// Kontrola ci had zjedol jablko
void eatApple() {
    int i;
    // Kontrola ci je pred hadom jablko
    if (isApple(snakeCords.rows[snakeCords.count - 1][0], snakeCords.rows[snakeCords.count - 1][1])) {
        // Ulozenie suradnic hada do docasneho pola
        int **temp;
        temp = (int **) malloc(snakeCords.count * sizeof(int) * 2);
        for (i = 0; i < snakeCords.count; ++i) {
            temp[i] = (int *) malloc(2 * sizeof(int));
        }
        for (i = 0; i < snakeCords.count; ++i) {
            temp[i][0] = snakeCords.rows[i][0];
            temp[i][1] = snakeCords.rows[i][1];
        }

        // Uvolnenie pamete pola suradnic hada
        free(snakeCords.rows);
        score++;
        // Zvacsovanie dlzky pola suradnic hada
        snakeCords.count++;
        // Alokovanie pamete pre noveho hada
        snakeCords.rows = (int **) malloc(snakeCords.count * sizeof(int) * 2);
        for (i = 0; i < snakeCords.count; ++i) {
            snakeCords.rows[i] = (int *) malloc(2 * sizeof(int));
        }
        // Presunutie docasnych suradnic do noveho pola
        for (i = 1; i < snakeCords.count; ++i) {
            snakeCords.rows[i][0] = temp[i - 1][0];
            snakeCords.rows[i][1] = temp[i - 1][1];
        }

        // Kontrola smeru hada na zaklade predposlednej bunky podla ktore bude zvoleny chvost
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

        // Generovanie nahodnych cisel
        srand(1000 * (unsigned) time(NULL));
        int index = 0;

        // Vyhladanie indexu zjedeneho jablka
        for (i = 0; i < 8; ++i) {
            if (snakeCords.rows[snakeCords.count - 1][0] == appleCords[i][0] &&
                snakeCords.rows[snakeCords.count - 1][1] == appleCords[i][1]) {
                index = i;
                break;
            }
        }
        // Zobrazenie jablka na novom mieste kde neni had ani prekazka
        do {
            appleCords[index][0] = rand() % 48 + 1;
            appleCords[index][1] = rand() % 23 + 1;
        } while (isObstacle(appleCords[i][0], appleCords[i][1]) ||
                 isSnake(appleCords[i][0], appleCords[i][1]));


        cords.X = (short) appleCords[index][0];
        cords.Y = (short) appleCords[index][1];
        c[0] = APPLE;
        c[1] = '\0';
        // Tlacenie noveho jablka
        WriteConsoleOutputCharacter(hStdout, c, 1, cords, &charsWritten);
        updateScore();
    }
}