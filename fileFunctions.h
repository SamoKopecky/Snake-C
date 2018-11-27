struct player {
    char name[10];
    int score;
};

void getValuesForFileFunctions(int ptrObstacleArray[52][2], int *ptrObstacleArrayLength, struct player *ptrCurrentPlayer,
                               struct player ptrArrayOfTop10player[10], int *ptrScore, char ptrPlayerName[10]);

void addCurrentPlayerToTop10(int indexOfPlayer);

void updateTop10List();

void importObstacles();