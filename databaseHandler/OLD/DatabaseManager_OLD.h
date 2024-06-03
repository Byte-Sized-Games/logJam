#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <sqlite3.h>
#include <string>

class DatabaseManager_OLD {
public:
    DatabaseManager_OLD();
    ~DatabaseManager_OLD();
    static int createDB();
    static int createTable();
    static void insertData(int level, int score, const std::string& player);
    static void outputData();
    static int callback(void* NotUsed, int argc, char** argv, char** azColName);
    static void deleteData(int id);
    static int getHiscore(const std::string& player, int level);
    static void genLB(int level);

private:
    const char* s;
    sqlite3* DB;
    static const char* dir;  // New member variable
};

#endif //DATABASEMANAGER_H