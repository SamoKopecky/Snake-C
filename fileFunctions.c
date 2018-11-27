#include <stdio.h>
#include "fileFunctions.h"

#define READ_FILE "prekazky.txt"
#define WRITE_FILE "rebricek_hracov.txt"

int obstacleArray[52][2];
int obstacleArrayLength;
struct player currentPlayer;
struct player arrayOfTop10player[10];
int score;
char playerName[10];

void getValuesForFileFunctions(int ptrObstacleArray[52][2], int *ptrObstacleArrayLength,
                               struct player *ptrCurrentPlayer, struct player ptrArrayOfTop10player[10], int *ptrScore,
                               char ptrPlayerName[10]) {
    obstacleArrayLength = *ptrObstacleArrayLength;
    for (int i = 0; i < 52; ++i) {
        for (int j = 0; j < 2; ++j) {
            obstacleArray[i][j] = ptrObstacleArray[i][j];
        }
    }
    currentPlayer = *ptrCurrentPlayer;
    score = *ptrScore;
    for (int l = 0; l < 10; ++l) {
        arrayOfTop10player[l] = ptrArrayOfTop10player[l];
    }
    for (int k = 0; k < 10; ++k) {
        playerName[k] = ptrPlayerName[k];
    }
}

// Importovanie prekazok zo suboru
void importObstacles() {
    FILE *file;
    file = fopen(READ_FILE, "r");
    for (int i = 0; i < obstacleArrayLength; ++i) {
        fscanf(file, "%d,%d", &obstacleArray[i][0], &obstacleArray[i][1]);

    }
    fclose(file);
}

// Pridanie skore hraca do pola top 10 hracov
void addCurrentPlayerToTop10(int indexOfPlayer) {
    int i;
    // Vymazanie posldneho hraca
    for (i = 9; i >= indexOfPlayer; --i) {
        arrayOfTop10player[i + 1] = arrayOfTop10player[i];
    }
    arrayOfTop10player[indexOfPlayer].score = currentPlayer.score;
    for (i = 0; i < 10; ++i) {
        arrayOfTop10player[indexOfPlayer].name[i] = currentPlayer.name[i];
    }
}

// Zapis top 10 hracov do suboru
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

    // Porovnanie skore hracov top 10 hracov z aktualnym hracom
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

