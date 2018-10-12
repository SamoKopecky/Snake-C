#include <stdio.h>
#include <stdbool.h>
#include <time.h>

char playground[10][10];
int snakeY = 0;
int snakeX = 0;

void createPlayground() {
    for (int x = 0; x < sizeof(playground)/ sizeof(playground[0]); ++x) {
        for (int y = 0; y < sizeof(playground[0]); ++y) {
            playground[x][y] = '*';
        }
    }
}

void printPlayground() {
    for (int x = 0; x < sizeof(playground)/ sizeof(playground[0]); ++x) {
        printf("\n");
        for (int y = 0; y < sizeof(playground[0]); ++y) {
            printf("%c", playground[x][y]);
        }
    }
    printf("\n");
}

void updateSnake() {
    snakeY++;
    snakeX++;

}

void updatePlayground() {
    for (int x = 0; x < sizeof(playground)/ sizeof(playground[0]); ++x) {
        for (int y = 0; y < sizeof(playground[0]); ++y) {
            if (y == snakeY && x == snakeX) {
                playground[x][y] = 'O';
            } else {
                playground[x][y] = '*';
            }
        }
    }
}

int main() {
    updatePlayground();
    printPlayground();

    unsigned long currentTime = time(NULL) + 1;

    while(true) {
        if (currentTime == time(NULL)) {
            updateSnake();
            updatePlayground();
            printPlayground();
            currentTime = time(NULL) + 1;
        }
    }

}