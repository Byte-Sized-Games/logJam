#include <iostream>
#include "DatabaseManager.h"

int main() {
    DatabaseManager dbManager;

    DatabaseManager::createDB();
    DatabaseManager::createTable();

    /*
    int level = 1;
    int score = 10000;
    std::string player = "mr league of legends";
    DatabaseManager::insertData(level, score, player);
     */

    DatabaseManager::outputData();

    DatabaseManager::genLB(3);

    return 0;
}