#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <afxres.h>
#include "fileFunctions.h"
#include "initializationOfGame.h"

// Deklaracia definov
#define HORIZONTAL_LINE (char) 205
#define VERTICAL_LINE (char) 186
#define UPPER_RIGHT_CORNER (char) 187
#define LOWER_RIGHT_CORNER (char) 188
#define UPPER_LEFT_CORNER (char) 201
#define LOWER_LEFT_CORNER (char) 200

// Deklaracia premennych
char playground[25][50];
int playgroundHeight = sizeof(playground) / sizeof(playground[0]);
int playgroundWidth = sizeof(playground[0]) / sizeof(playground[0][0]);
int obstacleArray[52][2];
int obstacleArrayLength = sizeof(obstacleArray) / sizeof(obstacleArray[0]);
int appleCords[8][2];
int appleCordsLength = sizeof(appleCords) / sizeof(appleCords[0]);
struct twoDArray snakeCords;
bool gameOver = false;
int score = 0;
char playerName[10];
int diff;
HANDLE hStdout;
char direction;
char inputKey;
struct player currentPlayer;
struct player arrayOfTop10player[10];

// Funkcia ktora vypise top 10 hracov na konci hry
void writeOutTop10() {
    printf("\n");
    for (int i = 0; i < 10; ++i) {
        printf("%s dosiahol skore : %d\n", arrayOfTop10player[i].name, arrayOfTop10player[i].score);
    }
}

void printGameOver() {
    // Vypisanie GAME OVER
    char line[12];
    for (int i = 0; i < 11; ++i) {
        line[i] = (char) HORIZONTAL_LINE;
    }
    line[11] = '\0';
    printf("%c%s%c", UPPER_LEFT_CORNER, line, UPPER_RIGHT_CORNER);
    printf("\n%c GAME OVER %c\n", VERTICAL_LINE, VERTICAL_LINE);
    printf("%c%s%c", LOWER_LEFT_CORNER, line, LOWER_RIGHT_CORNER);
}

int main() {
    // Posielanie premenych to kniznic
    getValuesForFileFunctions(obstacleArray, &obstacleArrayLength, &currentPlayer, arrayOfTop10player, &score, playerName);
    getValuesInitializationOfGame(playerName, &diff, &playgroundHeight, &playgroundWidth, playground, &score,
                                  &snakeCords, appleCords, &appleCordsLength);
    getValuesGameplayFunctions(&hStdout, &inputKey, &gameOver, &direction, &score, obstacleArray, &obstacleArrayLength,
                               &playgroundHeight, &playgroundWidth, &snakeCords, appleCords, &appleCordsLength);
    // Ulozenie premenej typu HANDLE z terminalu
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    // Volanie funkcii pre inicializaciu programu
    printInstructions();
    direction = 'd';
    firstAlloc();
    getPlayerName();
    getDifficulty();
    importObstacles();
    importApples();
    updatePlayground();
    // Definovanie struktury pre casovac
    struct timespec tim;
    tim.tv_sec = 0;
    // Zvolenie obtiaznosti hry z roznou rychlostou hada
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
    // Cyklus hry
    while (true) {
        // Zaznamenavanie stlacenia klavesy pre pohyb hada
        if (kbhit()) {
            inputKey = getch();
        }
        // Casovac
        nanosleep(&tim, NULL);
        // Pohyb v poli
        updateSnake();
        // Kontrola ci nenastal dovod pre ukoncenie hry
        if (gameOver) {
            break;
        }
        // Kontrola ci had zjedol jablko
        eatApple();
        // Pohyb v terminali
        printSnake();
    }
    // Vymazanie konzoli po ukonceni cyklu hry
    system("cls");
    // Vypisanie GAME OVER
    printGameOver();
    // Aktualizacia suboru z top 10 hracmi
    updateTop10List();
    // Vypisanie top 10 hracov do konzoli
    writeOutTop10();
    getchar();
}