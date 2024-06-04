//
// Created by  Brandon Thomas on 2024-06-03.
//

#ifndef LOGJAM_MAPDATA_H
#define LOGJAM_MAPDATA_H

#include "DatabaseManager.h"

class MapData : public DatabaseManager {
public:
    MapData();
    ~MapData();
    int createTable() override;
    std::string getCreateTableSQL() override;
    void insertData(const std::string& songTitle, const std::string& songArtist,
                    int length, int bpm, int difficulty, const std::string& source);
    //i know its a gross amount of parameters, but it doesnt make sense any other way
    static int callback(void* NotUsed, int argc, char** argv, char** azColName);
    void outputData();
    void deleteData(int id);
    void setMapDataDir();
    void nextLv();
    void prevLv();
    void displayLevel();
    int currentLevelId;
    int getMaxId();
    int getMinId();
    bool tryDisplayLevel(int id, const std::string& sql, sqlite3_stmt*& stmt);
};

#endif //LOGJAM_MAPDATA_H