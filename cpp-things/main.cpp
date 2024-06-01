#include "DatabaseManager.h"

int main() {
    const char* dir = "../leaderboards.db";
    DatabaseManager dbManager(dir);

    DatabaseManager::createDB(dir);
    DatabaseManager::createTable(dir);

    int level = 1;
    int score = 100;
    std::string player = "Player1";
    int time = 120;

    //ooo first time with new header system
    DatabaseManager::insertData(dir, level, score, player, time);

    return 0;
}