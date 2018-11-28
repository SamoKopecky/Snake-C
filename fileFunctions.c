#include "fileFunctions.h"

// Importovanie prekazok zo suboru
void importObstacles(struct coordinates *coordinates) {
    FILE *file;
    file = fopen(READ_FILE, "r");
    for (short i = 0; i < NUM_OF_OBSTACLES; ++i) {
        fscanf(file, "%hu,%hu", &coordinates->obstacleArray[i][0], &coordinates->obstacleArray[i][1]);

    }
    fclose(file);
}

// Pridanie skore hraca do pola top 10 hracov
void addCurrentPlayerToTop10(short indexOfPlayer, struct playerInfo *playerInfo) {
    short i;
    // Vymazanie posldneho hraca
    for (i = 9; i >= indexOfPlayer; --i) {
        playerInfo->arrayOfTop10Players[i + 1] = playerInfo->arrayOfTop10Players[i];
    }
    playerInfo->arrayOfTop10Players[indexOfPlayer].score = playerInfo->currentPlayer.score;
    for (i = 0; i < 10; ++i) {

        playerInfo->arrayOfTop10Players[indexOfPlayer].name[i] = playerInfo->currentPlayer.name[i];
    }
}

// Zapis top 10 hracov do suboru
void updateTop10List(struct playerInfo *playerInfo) {
    short i;

    FILE *file;
    file = fopen(WRITE_FILE, "r");
    for (i = 0; i < 10; ++i) {
        fscanf(file, "%s ", playerInfo->arrayOfTop10Players[i].name);
        fseek(file, 17, SEEK_CUR);
        fscanf(file, "%hu", &playerInfo->arrayOfTop10Players[i].score);

    }

    // Porovnanie skore hracov top 10 hracov z aktualnym hracom
    for (i = 0; i < 10; ++i) {
        if (playerInfo->arrayOfTop10Players[i].score < playerInfo->currentPlayer.score) {
            addCurrentPlayerToTop10(i, playerInfo);
            break;
        }
    }
    fclose(file);
    file = fopen(WRITE_FILE, "w");
    for (i = 0; i < 10; ++i) {
        fprintf(file, "%s dosiahol skore : %hu\n", playerInfo->arrayOfTop10Players[i].name,
                playerInfo->arrayOfTop10Players[i].score);
    }
    fclose(file);
    // Vypisanie hracov na terminal
    printf("\n");
    for (i = 0; i < 10; ++i) {
        printf("%s dosiahol skore : %hu\n", playerInfo->arrayOfTop10Players[i].name, playerInfo->arrayOfTop10Players[i].score);
    }
}

