#include <stdio.h>

#define READ_FILE "prekazky.txt"
#define WRITE_FILE "rebricek_hracov.txt"
#define NUM_OF_OBSTACLES 52
#define NUM_OF_APPLES 8
#define SNAKE_LENGTH_ON_START 4
#define OBSTACLE (char) 177
#define APPLE (char) 254
#define SNAKE (char) 248
#define HORIZONTAL_LINE (char) 205
#define VERTICAL_LINE (char) 186
#define UPPER_RIGHT_CORNER (char) 187
#define LOWER_RIGHT_CORNER (char) 188
#define UPPER_LEFT_CORNER (char) 201
#define LOWER_LEFT_CORNER (char) 200
#define PLAYGROUND_WIDTH 50
#define PLAYGROUND_HEIGHT 25

struct player {
    char name[10];
    short score;
};

struct twoDArray {
    short count;
    short **rows;
};

struct playerInfo {
    struct player currentPlayer;
    struct player arrayOfTop10Players[10];
};

struct coordinates {
    char playground[PLAYGROUND_HEIGHT][PLAYGROUND_WIDTH];
    short obstacleArray[NUM_OF_OBSTACLES][2];
    short appleCords[NUM_OF_APPLES][2];
    struct twoDArray snakeCords;
};

struct playerInput {
    short difficulty;
    char direction;
    char inputKey;
};

void addCurrentPlayerToTop10(short indexOfPlayer, struct playerInfo *playerInfo);

void updateTop10List(struct playerInfo *playerInfo);

void importObstacles(struct coordinates *coordinates);