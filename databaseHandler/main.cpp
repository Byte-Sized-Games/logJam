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

done???  -command line support
 -search and sort instead of if statements

 -review and comment code
 */



void handleCommandLineArguments(int argc, char* argv[]) {
    Leaderboards lb;
    MapData md;

    // Check if at least one argument is provided
    if (argc > 1) {
        // The first argument is the command
        std::string command = argv[1];

        if (command == "deleteData") {
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
        } else if (command == "outputData") {
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
        } else if (command == "getHiscore") {
            if (argc == 5) {
                std::string player = argv[2];
                int level = std::stoi(argv[3]);
                std::cout << "Hiscore: " << lb.getHiscore(player, level) << "\n";
            } else {
                std::cerr << "Usage: getHiscore <player> <level>\n";
            }
        } else if (command == "insertData") {
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
        } else if (command == "genLB") {
            if (argc == 4) {
                int level = std::stoi(argv[2]);
                char timeRange = argv[3][0];
                lb.genLB(level, timeRange);
            } else {
                std::cerr << "Usage: genLB <level> <timeRange>\n";
            }
        } else if (command == "nextLv") {
            if (argc == 2) {
                md.nextLv();
            } else {
                std::cerr << "Usage: nextLv\n";
            }
        } else if (command == "prevLv") {
            if (argc == 2) {
                md.prevLv();
            } else {
                std::cerr << "Usage: prevLv\n";
            }
        } else if (command == "next10Lv") {
            if (argc == 2) {
                md.next10Lv();
            } else {
                std::cerr << "Usage: next10Lv\n";
            }
        } else if (command == "prev10Lv") {
            if (argc == 2) {
                md.prev10Lv();
            } else {
                std::cerr << "Usage: prev10Lv\n";
            }
        } else if (command == "displayLevel") {
            if (argc == 2) {
                md.displayLevel();
            } else {
                std::cerr << "Usage: displayLevel\n";
            }
        } else if (command == "getMaxId") {
            if (argc == 2) {
                std::cout << "Max ID: " << md.getMaxId() << "\n";
            } else {
                std::cerr << "Usage: getMaxId\n";
            }
        } else if (command == "getMinId") {
            if (argc == 2) {
                std::cout << "Min ID: " << md.getMinId() << "\n";
            } else {
                std::cerr << "Usage: getMinId\n";
            }
        } else if (command == "getCurrentId") {
            if (argc == 2) {
                std::cout << "Current ID: " << md.getCurrentId() << "\n";
            } else {
                std::cerr << "Usage: getCurrentId\n";
            }
        } else if (command == "getCurrentLevel") {
            if (argc == 2) {
                std::cout << "Current Level: " << md.getCurrentLevel() << "\n";
            } else {
                std::cerr << "Usage: getCurrentLevel\n";
            }
        } else if (command == "isDuplicate") {
            if (argc == 3) {
                int level = std::stoi(argv[2]);
                std::cout << "Is Duplicate: " << (md.isDuplicate(level) ? "Yes" : "No") << "\n";
            } else {
                std::cerr << "Usage: isDuplicate <level>\n";
            }
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