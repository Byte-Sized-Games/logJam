#include <iostream>
#include "Leaderboards.h"
#include "MapData.h"

using namespace std;

/*
 things i need to do

DONE -mapData display song
DONE -mapData get song/next/previous
DONE -mapData get next x 10 or something
DONE -Leaderboards, sort with respect to time, day, week, month
DONE -mapData get id, so id parameter can be fed into generateLB
lmao no thanks -mapData changesort    **** bonus

 -command line support

 -review and comment code
 */

int main() {
    Leaderboards lb;

    lb.insertData(1,9180,"beandon");

    // Test genLB with a specific time range
    std::cout << "Leaderboard for level 1, past day:\n";
    lb.genLB(1, 'd');

    // Test genLB without a specific time range (defaults to all time)
    std::cout << "\nLeaderboard for level 1, all time:\n";
    lb.genLB(1);

    return 0;
}