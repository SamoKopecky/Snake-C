#include "gameplayFunctions.h"

COORD cords;
short oldSnakeCords[2];
unsigned long charsWritten;
char c[2];

// Kontrola jablka na suradniciach x a y
bool isApple(short x, short y, struct coordinates *coordinates) {
    for (short i = 0; i < NUM_OF_APPLES; ++i) {
        if (coordinates->appleCords[i][0] == x && coordinates->appleCords[i][1] == y) {
            return true;
        }
    }
    return false;
}

// Kontrola okraja hracej plochy na suradniciach x a y
bool isBorder(short x, short y) {
    if (x == 0 || x == PLAYGROUND_WIDTH - 1 || y == 0 || y == PLAYGROUND_HEIGHT - 1) {
        return true;
    }
    return false;
}

// Kontrola hada na suradniciach x a y
bool isSnake(short x, short y, struct coordinates *coordinates) {
    for (short i = 0; i < coordinates->snakeCords.count; ++i) {
        if (x == coordinates->snakeCords.rows[i][0] && y == coordinates->snakeCords.rows[i][1]) {
            return true;
        }
    }
    return false;
}

// Kontrola prekazky na suradniciach x a y
bool isObstacle(short x, short y, struct coordinates *coordinates) {
    for (short i = 0; i < NUM_OF_OBSTACLES; ++i) {
        if (coordinates->obstacleArray[i][0] == x && coordinates->obstacleArray[i][1] == y) {
            return true;
        }
    }
    return false;
}

// Kontrola tela hada na suradniciach x a y
bool isSnakeBody(short x, short y, struct coordinates *coordinates) {
    for (short i = 0; i < coordinates->snakeCords.count - 2; ++i) {
        if (coordinates->snakeCords.rows[i][0] == x && coordinates->snakeCords.rows[i][1] == y) {
            return true;
        }
    }
    return false;
}

// Tlacenie hada v terminali
void printSnake(struct coordinates *coordinates, HANDLE *hStdout) {
    cords.X = coordinates->snakeCords.rows[coordinates->snakeCords.count - 1][0];
    cords.Y = coordinates->snakeCords.rows[coordinates->snakeCords.count - 1][1];
    c[0] = SNAKE;
    c[1] = '\0';
    // Vytlaci novu hlavu hada
    WriteConsoleOutputCharacter(*hStdout, c, 1, cords, &charsWritten);
    cords.X = oldSnakeCords[0];
    cords.Y = oldSnakeCords[1];
    // Zmaze stary cvhost hada
    WriteConsoleOutputCharacter(*hStdout, " ", 1, cords, &charsWritten);
}

// Zmena smeru pohybu hada
void changeDirection(struct playerInput *playerInput) {
    // Zabranie cuvania hada
    if (!((playerInput->inputKey == 'w' && playerInput->direction == 's') ||
          (playerInput->inputKey == 's' && playerInput->direction == 'w') ||
          (playerInput->inputKey == 'd' && playerInput->direction == 'a') ||
          (playerInput->inputKey == 'a' && playerInput->direction == 'd'))) {
        if (playerInput->inputKey == 'w') {
            playerInput->direction = 'w';
        }
        if (playerInput->inputKey == 's') {
            playerInput->direction = 's';
        }
        if (playerInput->inputKey == 'd') {
            playerInput->direction = 'd';
        }
        if (playerInput->inputKey == 'a') {
            playerInput->direction = 'a';
        }
    }
}

// Funkcie pre aktualizaciu suradnic hada
void updateSnake(struct coordinates *coordinates, struct playerInput *playerInput, bool *gameOver) {
    // Ulozenie starych suradnic do docasneho pola
    oldSnakeCords[0] = coordinates->snakeCords.rows[0][0];
    oldSnakeCords[1] = coordinates->snakeCords.rows[0][1];
    changeDirection(playerInput);
    // Posunutie suradnic kazdej bunky hada
    for (short i = 0; i <= coordinates->snakeCords.count - 2; ++i) {
        coordinates->snakeCords.rows[i][0] = coordinates->snakeCords.rows[i + 1][0];
        coordinates->snakeCords.rows[i][1] = coordinates->snakeCords.rows[i + 1][1];
    }
    // Zmena smeru
    if (playerInput->direction == 'w') {
        coordinates->snakeCords.rows[coordinates->snakeCords.count - 1][1]--;
    }
    if (playerInput->direction == 's') {
        coordinates->snakeCords.rows[coordinates->snakeCords.count - 1][1]++;
    }
    if (playerInput->direction == 'd') {
        coordinates->snakeCords.rows[coordinates->snakeCords.count - 1][0]++;
    }
    if (playerInput->direction == 'a') {
        coordinates->snakeCords.rows[coordinates->snakeCords.count - 1][0]--;
    }
    // Kontrola ci had narazil do steny hracej plochy alebo do seba alebo do prekazky
    if (isBorder(coordinates->snakeCords.rows[coordinates->snakeCords.count - 1][0],
                 coordinates->snakeCords.rows[coordinates->snakeCords.count - 1][1]) == true ||
        isObstacle(coordinates->snakeCords.rows[coordinates->snakeCords.count - 1][0],
                   coordinates->snakeCords.rows[coordinates->snakeCords.count - 1][1], coordinates) == true ||
        isSnakeBody(coordinates->snakeCords.rows[coordinates->snakeCords.count - 1][0],
                    coordinates->snakeCords.rows[coordinates->snakeCords.count - 1][1], coordinates) == true) {
        *gameOver = true;
    }
}

// Tlacenie skore
void updateScore(HANDLE *hStdout, struct playerInfo *playerInfo) {
    cords.X = 8;
    cords.Y = 25;
    sprintf(c, "%d", playerInfo->currentPlayer.score);
    WriteConsoleOutputCharacter(*hStdout, c, sizeof(c), cords, &charsWritten);
}

// Kontrola ci had zjedol jablko
void eatApple(struct coordinates *coordinates, HANDLE *hStdout, struct playerInfo *playerInfo) {
    short i;
    // Kontrola ci je pred hadom jablko
    if (isApple(coordinates->snakeCords.rows[coordinates->snakeCords.count - 1][0],
                coordinates->snakeCords.rows[coordinates->snakeCords.count - 1][1], coordinates)) {
        // Ulozenie suradnic hada do docasneho pola
        short **temp;
        temp = (short **) malloc(coordinates->snakeCords.count * sizeof(short) * 2);
        for (i = 0; i < coordinates->snakeCords.count; ++i) {
            temp[i] = (short *) malloc(2 * sizeof(short));
        }
        for (i = 0; i < coordinates->snakeCords.count; ++i) {
            temp[i][0] = coordinates->snakeCords.rows[i][0];
            temp[i][1] = coordinates->snakeCords.rows[i][1];
        }

        // Uvolnenie pamete pola suradnic hada
        free(coordinates->snakeCords.rows);
        playerInfo->currentPlayer.score++;
        // Zvacsovanie dlzky pola suradnic hada
        coordinates->snakeCords.count++;
        // Alokovanie pamete pre noveho hada
        coordinates->snakeCords.rows = (short **) malloc(coordinates->snakeCords.count * sizeof(short) * 2);
        for (i = 0; i < coordinates->snakeCords.count; ++i) {
            coordinates->snakeCords.rows[i] = (short *) malloc(2 * sizeof(short));
        }
        // Presunutie docasnych suradnic do noveho pola
        for (i = 1; i < coordinates->snakeCords.count; ++i) {
            coordinates->snakeCords.rows[i][0] = temp[i - 1][0];
            coordinates->snakeCords.rows[i][1] = temp[i - 1][1];
        }

        // Kontrola smeru hada na zaklade predposlednej bunky podla ktore bude zvoleny chvost
        if (coordinates->snakeCords.rows[2][0] - 1 == coordinates->snakeCords.rows[1][0]) {
            coordinates->snakeCords.rows[0][0] = coordinates->snakeCords.rows[1][0] - (short) 1;
            coordinates->snakeCords.rows[0][1] = coordinates->snakeCords.rows[1][1];
        } else if (coordinates->snakeCords.rows[2][0] + 1 == coordinates->snakeCords.rows[1][0]) {
            coordinates->snakeCords.rows[0][0] = coordinates->snakeCords.rows[1][0] + (short) 1;
            coordinates->snakeCords.rows[0][1] = coordinates->snakeCords.rows[1][1];
        } else if (coordinates->snakeCords.rows[2][1] - 1 == coordinates->snakeCords.rows[1][1]) {
            coordinates->snakeCords.rows[0][0] = coordinates->snakeCords.rows[1][0];
            coordinates->snakeCords.rows[0][1] = coordinates->snakeCords.rows[1][1] - (short) 1;
        } else if (coordinates->snakeCords.rows[2][1] + 1 == coordinates->snakeCords.rows[1][1]) {
            coordinates->snakeCords.rows[0][0] = coordinates->snakeCords.rows[1][0];
            coordinates->snakeCords.rows[0][1] = coordinates->snakeCords.rows[1][1] + (short) 1;
        }

        // Generovanie nahodnych cisel
        srand(1000 * (unsigned) time(NULL));
        short index = 0;

        // Vyhladanie indexu zjedeneho jablka
        for (i = 0; i < NUM_OF_APPLES; ++i) {
            if (coordinates->snakeCords.rows[coordinates->snakeCords.count - 1][0] == coordinates->appleCords[i][0] &&
                coordinates->snakeCords.rows[coordinates->snakeCords.count - 1][1] == coordinates->appleCords[i][1]) {
                index = i;
                break;
            }
        }
        // Zobrazenie jablka na novom mieste kde neni had ani prekazka
        do {
            coordinates->appleCords[index][0] = (short) (rand() % 48 + 1);
            coordinates->appleCords[index][1] = (short) (rand() % 23 + 1);
        } while (isObstacle(coordinates->appleCords[i][0], coordinates->appleCords[i][1], coordinates) ||
                 isSnake(coordinates->appleCords[i][0], coordinates->appleCords[i][1], coordinates));


        cords.X = coordinates->appleCords[index][0];
        cords.Y = coordinates->appleCords[index][1];
        c[0] = APPLE;
        c[1] = '\0';
        // Tlacenie noveho jablka
        WriteConsoleOutputCharacter(*hStdout, c, 1, cords, &charsWritten);
        updateScore(hStdout, playerInfo);
    }

}