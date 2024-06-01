#include "DatabaseManager.h"
#include <iostream>
#include <cstdio>
#include <sqlite3.h>
#include <string>

using namespace std;

DatabaseManager::DatabaseManager(const char* s) : s(s) {
    sqlite3_open(s, &DB);
}

DatabaseManager::~DatabaseManager() {
    sqlite3_close(DB);
}

int DatabaseManager::createDB(const char* s) {
    sqlite3* DB;
    int exit = 0;

    exit = sqlite3_open(s, &DB);

    sqlite3_close(DB);

    return 0;
}

int DatabaseManager::createTable(const char* s) {
    sqlite3 *DB;
    char *messageError;

    string sql = "CREATE TABLE IF NOT EXISTS LEADERBOARD("
                 "ID INTEGER PRIMARY KEY AUTOINCREMENT, " //key to get values
                 "LEVEL      INT NOT NULL, "
                 "SCORE     INT NOT NULL, "
                 "PLAYER       TEXT  NOT NULL, "
                 "TIME   INT);";

    try {
        int exit = 0;
        exit = sqlite3_open(s, &DB);
        // An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here
        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
        if (exit != SQLITE_OK) {
            cerr << "createTable function failed." << endl;
            sqlite3_free(messageError);
        } else
            cout << "Table created/accessed successfully" << endl;
        sqlite3_close(DB);
    }
    catch (const exception &e) {
        cerr << e.what();
    }

    return 0;
}

void DatabaseManager::insertData(const char* s, int level, int score, const std::string& player, int time) {
    sqlite3* DB;
    sqlite3_stmt* stmt;

    std::string sql = "INSERT INTO LEADERBOARD (LEVEL, SCORE, PLAYER, TIME) VALUES (?, ?, ?, ?);";

    int exit = sqlite3_open(s, &DB);

    if (exit != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(DB) << std::endl;
        return;
    }

    exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0);

    if (exit != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << std::endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, level);
    sqlite3_bind_int(stmt, 2, score);
    sqlite3_bind_text(stmt, 3, player.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, time);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(DB) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(DB);
}