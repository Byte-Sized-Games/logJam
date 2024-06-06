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
                    int length, int bpm, int difficulty, int level, const std::string& source);
    //i know its a gross amount of parameters, but it doesnt make sense any other way
    static int callback(void* NotUsed, int argc, char** argv, char** azColName);
    void outputData();
    void deleteData(int id);
    void setMapDataDir();
    void nextLv();
    void prevLv();
    int getCurrentId() const;
    int getCurrentLevel();
    void displayLevel();
    int getMaxId();
    int getMinId();
    bool isDuplicate(int level);
    bool tryDisplayLevel(int id, const std::string& sql, sqlite3_stmt*& stmt);
    void saveCurrentValue();
    void loadCurrentValue();
    void next10Lv();
    void prev10Lv();
    int displayLevelCallback(void *NotUsed, int argc, char **argv, char **azColName);

private:
    int currentLevelId;
};

#endif //LOGJAM_MAPDATA_H