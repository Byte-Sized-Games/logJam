#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <sqlite3.h>
#include <string>

class DatabaseManager {
public:
    DatabaseManager(const char* s);
    ~DatabaseManager();
    static int createDB(const char* s);
    static int createTable(const char* s);
    static void insertData(const char* s, int level, int score, const std::string& player, int time);

private:
    const char* s;
    sqlite3* DB;
};

#endif //DATABASEMANAGER_H