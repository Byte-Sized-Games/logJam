#include <iostream>
#include "Leaderboards.h"

int main() {
    Leaderboards lbManager;

    lbManager.createDB();
    lbManager.createTable();

    int level = 1;
    int score = 2000;
    std::string player = "beandon";
    lbManager.insertLeaderboardData(level, score, player);

    lbManager.outputData();

    lbManager.genLB(1);

    return 0;
}