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
unable to -mapData changesort    **** bonus

done???  -command line support
done?? -search and sort instead of if statements

 -review and comment code
 main: ~200 lines, mapData.cpp: ~310, Leaderboards.cpp: ~145, DatabaseManager: ~165,
 mapData.h: ~40, Leaderboards.h: ~25, DatabaseManager.h: ~40
 200+310+145+165+40+25+40=925 lines im sobbing
 */


//also uses recursion
int binarySearch(std::pair<std::string, std::function<void(int, char**)>> arr[], int l, int r, std::string x) {
    if (r >= l) {
        int mid = l + (r - l) / 2;

        // If the element is present at the middle
        if (arr[mid].first == x)
            return mid;

        // If element is smaller than mid, then it can only be present in left subarray
        if (arr[mid].first > x)
            return binarySearch(arr, l, mid - 1, x);

        // Else the element can only be present in right subarray
        return binarySearch(arr, mid + 1, r, x);
    }

    // We reach here when element is not present in array
    return -1;
}

void handleCommandLineArguments(int argc, char* argv[]) {
    Leaderboards lb;
    MapData md;

    lb.createDB();
    lb.createTable();
    md.createDB();
    md.createTable();

    // Define the command array
    std::pair<std::string, std::function<void(int, char**)>> commandArray[] = {
            //this uses binary search to search for the commands instead of many if statements
            {"deleteData", [&lb, &md](int argc, char* argv[]) {
                if (argc == 4) {
                    std::string className = argv[2];
                    int id = std::stoi(argv[3]);
                    if (className == "Leaderboards") {
                        lb.deleteData(id);
                    } else if (className == "MapData") {
                        md.deleteData(id);
                    }
                } else {
                    std::cerr << "Usage: deleteData <className> <id>\n";
                }
            }},
            {"outputData", [&lb, &md](int argc, char* argv[]) {
                if (argc == 3) {
                    std::string className = argv[2];
                    if (className == "Leaderboards") {
                        lb.outputData();
                    } else if (className == "MapData") {
                        md.outputData();
                    }
                } else {
                    std::cerr << "Usage: outputData <className>\n";
                }
            }},
            {"getHiscore", [&lb](int argc, char* argv[]) {
                if (argc == 5) {
                    std::string player = argv[2];
                    int level = std::stoi(argv[3]);
                    std::cout << "Hiscore: " << lb.getHiscore(player, level) << "\n";
                } else {
                    std::cerr << "Usage: getHiscore <player> <level>\n";
                }
            }},
            {"insertData", [&lb, &md](int argc, char* argv[]) {
                if (argc == 9) {
                    std::string className = argv[2];
                    if (className == "Leaderboards") {
                        int level = std::stoi(argv[3]);
                        int score = std::stoi(argv[4]);
                        std::string player = argv[5];
                        lb.insertData(level, score, player);
                    } else if (className == "MapData") {
                        std::string songTitle = argv[3];
                        std::string songArtist = argv[4];
                        int length = std::stoi(argv[5]);
                        int bpm = std::stoi(argv[6]);
                        int difficulty = std::stoi(argv[7]);
                        int level = std::stoi(argv[8]);
                        std::string source = "default";
                        md.insertData(songTitle, songArtist, length, bpm, difficulty, level, source);
                    }
                } else {
                    std::cerr << "Usage: insertData <className> <parameters>\n";
                }
            }},
            {"genLB", [&lb](int argc, char* argv[]) {
                if (argc == 4) {
                    int level = std::stoi(argv[2]);
                    char timeRange = argv[3][0];
                    lb.genLB(level, timeRange);
                } else {
                    std::cerr << "Usage: genLB <level> <timeRange>\n";
                }
            }},
            {"nextLv", [&md](int argc, char* argv[]) {
                if (argc == 2) {
                    md.nextLv();
                } else {
                    std::cerr << "Usage: nextLv\n";
                }
            }},
            {"prevLv", [&md](int argc, char* argv[]) {
                if (argc == 2) {
                    md.prevLv();
                } else {
                    std::cerr << "Usage: prevLv\n";
                }
            }},
            {"next10Lv", [&md](int argc, char* argv[]) {
                if (argc == 2) {
                    md.next10Lv();
                } else {
                    std::cerr << "Usage: next10Lv\n";
                }
            }},
            {"prev10Lv", [&md](int argc, char* argv[]) {
                if (argc == 2) {
                    md.prev10Lv();
                } else {
                    std::cerr << "Usage: prev10Lv\n";
                }
            }},
            {"displayLevel", [&md](int argc, char* argv[]) {
                if (argc == 2) {
                    md.displayLevel();
                } else {
                    std::cerr << "Usage: displayLevel\n";
                }
            }},
            {"getMaxId", [&md](int argc, char* argv[]) {
                if (argc == 2) {
                    std::cout << "Max ID: " << md.getMaxId() << "\n";
                } else {
                    std::cerr << "Usage: getMaxId\n";
                }
            }},
            {"getMinId", [&md](int argc, char* argv[]) {
                if (argc == 2) {
                    std::cout << "Min ID: " << md.getMinId() << "\n";
                } else {
                    std::cerr << "Usage: getMinId\n";
                }
            }},
            {"getCurrentId", [&md](int argc, char* argv[]) {
                if (argc == 2) {
                    std::cout << "Current ID: " << md.getCurrentId() << "\n";
                } else {
                    std::cerr << "Usage: getCurrentId\n";
                }
            }},
            {"getCurrentLevel", [&md](int argc, char* argv[]) {
                if (argc == 2) {
                    std::cout << "Current Level: " << md.getCurrentLevel() << "\n";
                } else {
                    std::cerr << "Usage: getCurrentLevel\n";
                }
            }},
            {"isDuplicate", [&md](int argc, char* argv[]) {
                if (argc == 3) {
                    int level = std::stoi(argv[2]);
                    std::cout << "Is Duplicate: " << (md.isDuplicate(level) ? "Yes" : "No") << "\n";
                } else {
                    std::cerr << "Usage: isDuplicate <level>\n";
                }
            }}
    };

    // Check if at least one argument is provided
    if (argc > 1) {
        // The first argument is the command
        std::string command = argv[1];

        // Perform a binary search for the command
        int index = binarySearch(commandArray, 0, sizeof(commandArray)/sizeof(commandArray[0]) - 1, command);

        // Check if the command exists in the array
        if (index != -1) {
            // Execute the command
            commandArray[index].second(argc, argv);
        } else {
            std::cerr << "Unknown command: " << command << "\n";
        }
    } else {
        std::cerr << "No command provided.\n";
    }
}

int main(int argc, char* argv[]) {
    handleCommandLineArguments(argc, argv);

    return 0;
}