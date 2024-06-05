#include <iostream>
#include "Leaderboards.h"
#include "MapData.h"

using namespace std;

/*
 things i need to do

DONE -mapData display song
DONE -mapData get song/next/previous
DONE -mapData get next x 10 or something
-Leaderboards, sort with respect to time, day, week, month
-mapData get id, so id parameter can be fed into generateLB
 -mapData changesort    **** bonus

 -command line support

 -review and comment code
 */

int main() {
    // Create a MapData object
    MapData mapData;
    // Test getCurrentLevel method
    int currentLevel = mapData.getCurrentLevel();
    std::cout << "Current level: " << currentLevel << std::endl;

    // Create a Leaderboards object
    Leaderboards leaderboards;

    // Create a leaderboard for the current level
    leaderboards.genLB(currentLevel);  // Pass currentLevel directly to genLB

    // Rest of your code...
}