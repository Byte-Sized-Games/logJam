#include <iostream>
#include "Leaderboards.h"
#include "MapData.h"

using namespace std;

/*
 things i need to do

 -mapData display song
-mapData get song/next/previous
-mapData get next x 10 or something
-Leaderboards, sort with respect to time, day, week, month
-mapData get id, so id parameter can be fed into generateLB
 -mapData changesort

 -command line support

 -review and comment code
 */

int main() {
    // Create a MapData object
    MapData mapData;

    // Test MapData methods
    mapData.createDB();
    mapData.createTable();

    mapData.outputData();

    // Display the current level
    std::cout << "Displaying current level:\n";
    mapData.displayLevel();

    // Go to the next level and display it
    std::cout << "Going to next level:\n";
    mapData.nextLv();

    // Go to the previous level and display it
    std::cout << "Going to previous level:\n";
    mapData.prevLv();

    return 0;
}