#include <stdio.h>
#include <time.h>

int playgroundX = 10;
int playgroundY = 10;

void createPlayground(char playground[][10]) {
    for (int x = 0; x < playgroundX; ++x) {
        for (int y = 0; y < playgroundY; ++y) {
            playground[x][y] = '*';
        }
    }
}

void printPlayground(char playground[][10]) {
    for (int x = 0; x < playgroundX; ++x) {
        printf("\n");
        for (int y = 0; y < playgroundY; ++y) {
            printf("%c", playground[x][y]);

        }
    }
}

int main() {
    char playground[10][10];
    createPlayground(playground);
    printPlayground(playground);
    //unsigned long currentTime = time(NULL);
    //do {

    //}while (true);


    return 0;
}