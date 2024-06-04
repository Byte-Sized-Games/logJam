#include <iostream>
#include "Leaderboards.h"

using namespace std;

int main() {
    Leaderboards boardHolder;

    boardHolder.createDB();
    boardHolder.createTable();


    int level = 1;
    int score = 2000;
    std::string player = "pqrs";
    boardHolder.insertData(level, score, player);


    boardHolder.outputData();

    cout << "\nhighscore for sdffasdfadf on lv5: " << boardHolder.getHiscore("yourmother69", 5) << endl;

    boardHolder.genLB(1);

    return 0;
}