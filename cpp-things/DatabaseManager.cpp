#include "DatabaseManager.h"
#include <iostream>
#include <cstdio>
#include <sqlite3.h>
#include <string>
#include <chrono>

using namespace std;

const char* DatabaseManager::dir = "../leaderboards.db";

DatabaseManager::DatabaseManager() {
    sqlite3_open(dir, &DB);
}

DatabaseManager::~DatabaseManager() {
    sqlite3_close(DB);
}

int DatabaseManager::createDB() {
    sqlite3* DB;
    int exit = 0;

    exit = sqlite3_open(dir, &DB);

    sqlite3_close(DB);

    return 0;
}

int DatabaseManager::createTable() {
    sqlite3 *DB;
    char *messageError;

    string sql = "CREATE TABLE IF NOT EXISTS LEADERBOARD("
                 "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                 "LEVEL      INT NOT NULL, "
                 "SCORE     INT NOT NULL, "
                 "PLAYER       TEXT  NOT NULL, "
                 "TIME   INT);";

    try {
        int exit = 0;
        exit = sqlite3_open(dir, &DB);
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

void DatabaseManager::insertData(int level, int score, const std::string& player) {
    sqlite3* DB;
    sqlite3_stmt* stmt;

    // Get the current time as a time_point object
    auto now = std::chrono::system_clock::now();

    // Convert the time_point object to a time_t object
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    std::string sql = "INSERT INTO LEADERBOARD (LEVEL, SCORE, PLAYER, TIME) VALUES (?, ?, ?, ?);";

    int exit = sqlite3_open(dir, &DB);

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
    sqlite3_bind_int(stmt, 4, static_cast<int>(currentTime));

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(DB) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(DB);
}

int DatabaseManager::callback(void* NotUsed, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        std::cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << "\n";
    }
    std::cout << "\n";
    return 0;
}

void DatabaseManager::outputData() {
    sqlite3* DB;
    char* messageError;
    std::string sql = "SELECT * FROM LEADERBOARD;";

    int exit = sqlite3_open(dir, &DB);

    if (exit != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(DB) << std::endl;
        return;
    }

    exit = sqlite3_exec(DB, sql.c_str(), callback, 0, &messageError);

    if (exit != SQLITE_OK) {
        std::cerr << "Failed to select data: " << sqlite3_errmsg(DB) << std::endl;
        sqlite3_free(messageError);
    }

    sqlite3_close(DB);
}

void DatabaseManager::deleteData(int id) {
    sqlite3* DB;
    sqlite3_stmt* stmt;

    std::string sql = "DELETE FROM LEADERBOARD WHERE ID = ?;";

    int exit = sqlite3_open(dir, &DB);

    if (exit != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(DB) << std::endl;
        return;
    }

    exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0);

    if (exit != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << std::endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(DB) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(DB);
}

int DatabaseManager::getHiscore(const std::string& player, int level) {
    sqlite3* DB;
    sqlite3_stmt* stmt;
    int hiscore = 0;

    std::string sql = "SELECT MAX(SCORE) FROM LEADERBOARD WHERE PLAYER = ? AND LEVEL = ?;";

    int exit = sqlite3_open(dir, &DB);

    if (exit != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(DB) << std::endl;
        return -1;
    }

    exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0);

    if (exit != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << std::endl;
        return -1;
    }

    sqlite3_bind_text(stmt, 1, player.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, level);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        hiscore = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(DB);

    return hiscore;
}

void DatabaseManager::genLB(int level) {
    sqlite3* DB;
    sqlite3_stmt* stmt;

    // Modify the SQL query to sort by TIME in ascending order as a secondary sorting criterion
    std::string sql = "SELECT PLAYER, MAX(SCORE), MIN(TIME) FROM LEADERBOARD WHERE LEVEL = ? GROUP BY PLAYER ORDER BY MAX(SCORE) DESC, MIN(TIME) ASC;";

    int exit = sqlite3_open(dir, &DB);

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

    std::cout << "Leaderboard for level " << level << ":\n";

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string player = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        int score = sqlite3_column_int(stmt, 1);

        std::cout << "Score: " << score << ", Player: " << player << "\n";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(DB);
}