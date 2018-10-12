#include <stdio.h>
#include <time.h>

void createPlayground(char playground[][]) {
    for (int x = 0; x < sizeof(playground/)/sizeof(playground[0]); ++x) {
        for (int y = 0; y < sizeof(playground[0]); ++y) {
            playground[x][y] = '*';
        }
    }
}

void printPlayground(char playground) {
    for (int x = 0; x < sizeof(playground)/sizeof(playground[0]); ++x) {
        printf("\n");
        for (int y = 0; y < sizeof(playground[0]); ++y) {
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