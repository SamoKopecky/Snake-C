#include <stdio.h>

int main() {
    char playground[15][15];

    for (int x = 0; x < sizeof(playground)/sizeof(playground[0]); ++x) {
        for (int y = 0; y < sizeof(playground[0]); ++y) {
            playground[x][y] = '*';
        }
    }

    for (int x = 0; x < sizeof(playground)/sizeof(playground[0]); ++x) {
        printf("\n");
        for (int y = 0; y < sizeof(playground[0]); ++y) {
            printf("%c", playground[x][y]);
        }
    }
    return 0;
}