#include <stdio.h>
#include <stdbool.h>
#include <time.h>

char playground[10][10];

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

int main() {
    createPlayground();
    printPlayground();
    unsigned long currentTime = time(NULL) + 1;

    while(true) {
        if (currentTime == time(NULL)) {
            currentTime = time(NULL) + 1;
            printPlayground();
        }
    }

}