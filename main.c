#include <stdio.h>
#include <stdbool.h>
#include <time.h>

char playground[25][50];
int snakeCords[2] = {2, 2};
int delay = 1;

void printPlayground() {
    for (int x = 0; x < sizeof(playground)/ sizeof(playground[0]); ++x) {
        printf("\n");
        for (int y = 0; y < sizeof(playground[0]) / sizeof(playground[0][0]); ++y) {
            printf("%c", playground[x][y]);
        }
    }
    printf("\n");
}

void updateSnake() {
    snakeCords[0]++;
    snakeCords[1]++;

}

bool checkForSnakeCoordinates(int x, int y) {
    if(x == snakeCords[0] && y == snakeCords[1]) {
        return true;
    } else {
        return false;
    }
}

void updatePlayground() {
    for (int x = 0; x < sizeof(playground)/ sizeof(playground[0]); ++x) {
        for (int y = 0; y < sizeof(playground[0]); ++y) {
            if (checkForSnakeCoordinates(x, y)) {
                playground[x][y] = 'O';
            } else {
                playground[x][y] = ' ';
            }
            if (x == 0 || x == sizeof(playground) / sizeof(playground[0]) - 1 || y == 0 || y == sizeof(playground[0]) - 1) {
                playground[x][y] = '*';
            }

        }
    }
}


int main() {
    updatePlayground();
    printPlayground();

    unsigned long currentTime = time(NULL) + delay;

    while(true) {
        if (currentTime == time(NULL)) {
            updateSnake();
            updatePlayground();
            printPlayground();
            currentTime = time(NULL) + delay;
        }
    }

}