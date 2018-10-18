#include <stdio.h>
#include <stdbool.h>
#include <time.h>

char playground[25][50];
int snakeLenght = 2;

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
    //snakeY++;
    //snakeX++;

}

bool checkForSnakeCoordinates(int x, int y) {
    return false;
};

void updatePlayground() {
    for (int x = 0; x < sizeof(playground)/ sizeof(playground[0]); ++x) {
        for (int y = 0; y < sizeof(playground[0]); ++y) {
            if (checkForSnakeCoordinates(x, y)) {
                playground[x][y] = 'O';
            }
            if (x == 0 || x == sizeof(playground) / sizeof(playground[0]) - 1 || y == 0 || y == sizeof(playground[0]) - 1) {
                playground[x][y] = '*';
            }
            else {
                playground[x][y] = ' ';
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