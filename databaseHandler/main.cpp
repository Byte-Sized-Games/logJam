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
    std::cout << "im alive\n";
    // Test MapData methods
    mapData.createDB();
    std::cout << "pain and suffering 5\n";
    mapData.createTable();

    // Insert a test record
    mapData.insertData("Test Song", "Test Artist", 200, 120, 5, 1, "Test Source");

    mapData.outputData();

    // Display the current level
    std::cout << "Displaying current level:\n";
    mapData.displayLevel();

    // Go to the next level and display it
    std::cout << "Going to next level:\n";
    mapData.nextLv();

    // Go to the next 10 levels and display it
    std::cout << "Going to next 10 levels:\n";
    mapData.next10Lv();

    // Go to the previous level and display it
    std::cout << "Going to next 10 levels again:\n";
    mapData.next10Lv();

    // Go to the previous 10 levels and display it
    std::cout << "Going to prev 10 levels:\n";
    mapData.prev10Lv();

    return 0;
}