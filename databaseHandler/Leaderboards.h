//
// Created by  Brandon Thomas on 2024-06-02.
//

#ifndef LOGJAM_LEADERBOARDS_H
#define LOGJAM_LEADERBOARDS_H

#include "DatabaseManager.h"

class Leaderboards : public DatabaseManager {
public:

    ~Leaderboards();
    Leaderboards();
    int createTable() override;
    std::string getCreateTableSQL() override;
    static int callback(void* NotUsed, int argc, char** argv, char** azColName);
    void deleteData(int id, const std::string& sql) override;
    void outputData();
    int getHiscore(const std::string& player, int level);
    void genLB(int level);
    void insertData(int level, int score, const std::string& player); //oooo overloading (so i can use the same insertData(...) )
};

#endif //LOGJAM_LEADERBOARDS_H
