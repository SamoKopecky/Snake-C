#include <conio.h>
#include "initializationOfGame.h"

// Deklaracia premennych
struct coordinates coordinates_value;
struct playerInfo playerInfo_value;
struct playerInput playerInput_value;

struct coordinates *coordinates = &coordinates_value;
struct playerInfo *playerInfo = &playerInfo_value;
struct playerInput *playerInput = &playerInput_value;

struct timespec tim;

HANDLE hStdout_value;
bool gameOver_value;

HANDLE *hStdout = &hStdout_value;
bool *gameOver = &gameOver_value;

void printGameOver() {
    // Vypisanie GAME OVER
    char line[12];
    for (int i = 0; i < 11; ++i) {
        line[i] = HORIZONTAL_LINE;
    }
    line[11] = '\0';
    printf("%c%s%c", UPPER_LEFT_CORNER, line, UPPER_RIGHT_CORNER);
    printf("\n%c GAME OVER %c\n", VERTICAL_LINE, VERTICAL_LINE);
    printf("%c%s%c", LOWER_LEFT_CORNER, line, LOWER_RIGHT_CORNER);
}

int main() {
    playerInfo->currentPlayer.score = 0;
    playerInput->direction = 'd';
    // Ulozenie premenej typu HANDLE z terminalu
    *hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    // Volanie funkcii pre inicializaciu programu
    printInstructions();
    firstAlloc(coordinates);
    getPlayerName(playerInfo);
    getDifficulty(playerInput);
    importObstacles(coordinates);
    importApples(coordinates);
    updatePlayground(coordinates, playerInfo);
    // Definovanie struktury pre casovac
    tim.tv_sec = 0;
    // Zvolenie obtiaznosti hry z roznou rychlostou hada a kontrolovanie vstupu
    switch (playerInput->difficulty) {
        case 1  :
            tim.tv_nsec = 250000000; // 0.25 sec
            break;
        case 2 :
            tim.tv_nsec = 125000000; // 0.125 sec
            break;
        case 3 :
            tim.tv_nsec = 100000000; // 0.1 sec
            break;
        default:
            tim.tv_nsec = 250000000;
            break;
    }
    // Cyklus hry
    while (true) {
        // Zaznamenavanie stlacenia klavesy pre pohyb hada
        if (kbhit()) {
            playerInput->inputKey = (char) getch();
        }
        // Casovac
        nanosleep(&tim, NULL);
        // Pohyb v poli
        updateSnake(coordinates, playerInput, gameOver);
        // Kontrola ci nenastal dovod pre ukoncenie hry
        if (*gameOver) {
            break;
        }
        // Kontrola ci had zjedol jablko
        eatApple(coordinates, hStdout, playerInfo);
        // Pohyb v terminali
        printSnake(coordinates, hStdout);
    }
    // Vymazanie konzoli po ukonceni cyklu hry
    system("cls");
    // Vypisanie GAME OVER
    printGameOver();
    // Aktualizacia suboru z top 10 hracmi
    updateTop10List(playerInfo);
    getchar();
}