#include "initializationOfGame.h"

// Nacitanie mena hraca
void getPlayerName(struct playerInfo *playerInfo) {
    printf("Zadaj svoje meno : ");
    scanf("%s", playerInfo->currentPlayer.name);
    while (getchar() != '\n');
}

// Vytlacenie instrukcii pre hru
void printInstructions() {
    printf("Pohyb : W -> hore, S -> dole, D -> doprava, A -> dolava.");
    printf("\nAk zjes jablko pripocita sa ti 1 k tvojmu skore.\n");
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
void getDifficulty(struct playerInput *playerInput) {
    printf("Zadaj obtiaznost : ");
    scanf("%hu", &playerInput->difficulty);
    while (getchar() != '\n');
    system("cls");
}

// Vybranie spravneho znaku pre prvotne tlacenie
char getCorrectCharacter(short x, short y, struct coordinates *coordinates) {
    if (isBorder(x, y)) {
        return getCorrectBorder(x, y);
    } else if (isApple(x, y, coordinates)) {
        return APPLE;
    } else if (isObstacle(x, y, coordinates)) {
        return OBSTACLE;
    } else if (isSnake(x, y, coordinates)) {
        return SNAKE;
    } else {
        return ' ';
    }
}

// Vrati znak podla toho ake je x a y
char getCorrectBorder(short x, short y) {
    if (x == 0 && y == 0) {
        return UPPER_LEFT_CORNER;
    } else if (x == 0 && y == PLAYGROUND_HEIGHT - 1) {
        return LOWER_LEFT_CORNER;
    } else if (x == PLAYGROUND_WIDTH - 1 && y == 0) {
        return UPPER_RIGHT_CORNER;
    } else if (x == PLAYGROUND_WIDTH - 1 && y == PLAYGROUND_HEIGHT - 1) {
        return LOWER_RIGHT_CORNER;
    } else if (x == 0 || x == PLAYGROUND_WIDTH - 1) {
        return VERTICAL_LINE;
    } else if (y == 0 || y == PLAYGROUND_HEIGHT - 1) {
        return HORIZONTAL_LINE;
    }
    return '*';
}

// Prvotne tlacenie hracej plochy
void updatePlayground(struct coordinates *coordinates, struct playerInfo *playerInfo) {
    for (short y = 0; y < PLAYGROUND_HEIGHT; ++y) {
        for (short x = 0; x < PLAYGROUND_WIDTH; ++x) {
            coordinates->playground[y][x] = getCorrectCharacter(x, y, coordinates);
        }
    }
    for (short y = 0; y < PLAYGROUND_HEIGHT; ++y) {
        if (y != 0) {
            printf("\n");
        }
        for (short x = 0; x < PLAYGROUND_WIDTH; ++x) {
            printf("%c", coordinates->playground[y][x]);
        }
    }
    printf("\nSCORE : %hu", playerInfo->currentPlayer.score);
}

// Prva alokacia pamete pola pre suradnice hada
void firstAlloc(struct coordinates *coordinates) {
    short i;
    coordinates->snakeCords.count = SNAKE_LENGTH_ON_START;
    coordinates->snakeCords.rows = (short **) malloc(SNAKE_LENGTH_ON_START * sizeof(short) * 2);
    for (i = 0; i < coordinates->snakeCords.count; ++i) {
        coordinates->snakeCords.rows[i] = (short *) malloc(2 * sizeof(short));
    }

    // Prvotna poloha hada
    for (i = 0; i < coordinates->snakeCords.count; ++i) {
        coordinates->snakeCords.rows[i][0] = i + (short) 2;
        coordinates->snakeCords.rows[i][1] = 2;
    }
}

// Importovanie jablk do pola suradnic jablk
void importApples(struct coordinates *coordinates) {
    srand(1000 * (unsigned) time(NULL));
    for (short i = 0; i < NUM_OF_APPLES; ++i) {
        coordinates->appleCords[i][0] = (short) (rand() % 48 + 1);
        coordinates->appleCords[i][1] = (short) (rand() % 23 + 1);
        // Kontrola ak su suradnice jablka take iste ako suradnice prekazky alebo hada a ak ano tak odcita od i
        if (isObstacle(coordinates->appleCords[i][0], coordinates->appleCords[i][1], coordinates) ||
            isSnake(coordinates->appleCords[i][0], coordinates->appleCords[i][1], coordinates)) {
            i--;
        }
    }
}