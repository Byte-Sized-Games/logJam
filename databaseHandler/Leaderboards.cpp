//
// Created by  Brandon Thomas on 2024-06-02.
//

#include "Leaderboards.h"

Leaderboards::Leaderboards(){

}

Leaderboards::~Leaderboards() {
    // Destructor implementation
}

int Leaderboards::createTable() {
    return DatabaseManager::createTable();
}

std::string Leaderboards::getCreateTableSQL() {
    return "CREATE TABLE IF NOT EXISTS LEADERBOARD("
           "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
           "LEVEL      INT NOT NULL, "
           "SCORE     INT NOT NULL, "
           "PLAYER       TEXT  NOT NULL, "
           "TIME   INT);";
}

int Leaderboards::callback(void* NotUsed, int argc, char** argv, char** azColName) {
    for(int i = 0; i < argc; i++) {
        std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << "\n";
    }
    std::cout << "\n";
    return 0;
}

void Leaderboards::deleteData(int id, const std::string& sql) {
    std::string deleteSql = "DELETE FROM LEADERBOARD WHERE ID = ?;";

    DatabaseManager::deleteData(id, deleteSql);
}

void Leaderboards::outputData() {
    sqlite3* DB;
    char* messageError;
    std::string sql = "SELECT * FROM LEADERBOARD;";

    int exit = sqlite3_open(dir, &DB);

    checkOpenDatabase(exit);

    exit = sqlite3_exec(DB, sql.c_str(), Leaderboards::callback, 0, &messageError);

    if (exit != SQLITE_OK) {
        std::cerr << "Failed to select data: " << sqlite3_errmsg(DB) << std::endl;
        sqlite3_free(messageError);
    }

    sqlite3_close(DB);
}

int Leaderboards::getHiscore(const std::string& player, int level) {
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

void Leaderboards::genLB(int level) {
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

void Leaderboards::insertData(int level, int score, const std::string& player) {
    std::string sql = "INSERT INTO LEADERBOARD (LEVEL, SCORE, PLAYER, TIME) VALUES (?, ?, ?, ?);";
    auto bindFunc = [level, score, player](sqlite3_stmt* stmt) {
        sqlite3_bind_int(stmt, 1, level);
        sqlite3_bind_int(stmt, 2, score);
        sqlite3_bind_text(stmt, 3, player.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 4, std::time(0)); // Unix timestamp
    };
    insertDataHelper(sql, bindFunc);
}

