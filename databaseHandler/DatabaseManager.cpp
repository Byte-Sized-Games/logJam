#include "DatabaseManager.h"

using namespace std;

const char* DatabaseManager::dir = "../leaderboards.db";

DatabaseManager::DatabaseManager() {
    sqlite3_open(dir, &DB);
}

int DatabaseManager::createDB() {
    sqlite3* DB;
    int exit = 0;
    sqlite3_open(dir, &DB);
    sqlite3_close(DB);
    return 0;
}

void DatabaseManager::setDir(const char* directory) {
    dir = directory;
}

//theres a lot that can be shared between leaderboards and mapdata, so use inheritance
//its just the sql query thats different
int DatabaseManager::createTable() {
    sqlite3 *DB;
    char *messageError;
    std::string sql = getCreateTableSQL();
    try {
        int exit = sqlite3_open(this->dir, &DB);  // Open the database
        checkOpenDatabase(exit);

        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
        if (exit != SQLITE_OK) {
            std::cerr << "createTable function failed." << std::endl;
            sqlite3_free(messageError);
        }

        sql = "CREATE TABLE IF NOT EXISTS CurrentValue (ID INTEGER PRIMARY KEY, Value INTEGER);";
        sqlite3_stmt* stmt;
        exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0);
        checkPrepareStatement(exit);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        sql = "INSERT OR IGNORE INTO CurrentValue (ID, Value) VALUES (1, 0);";
        exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0);
        checkPrepareStatement(exit);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        sqlite3_close(DB);
    }
    catch (const exception &e) {
        cerr << e.what();
    }
    return 0;
}

//it has to do checks and generate a statement since we're inserting variables
void DatabaseManager::executeSQLWithCallback(const std::string& sql, std::function<void(sqlite3_stmt*)> bindFunc, std::function<void(sqlite3_stmt*)> callback) {
    sqlite3* DB;
    sqlite3_stmt* stmt;

    int exit = sqlite3_open(this->dir, &DB);
    checkOpenDatabase(exit);

    exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0);
    checkPrepareStatement(exit);

    // Call the bind function, if provided
    if (bindFunc) {
        bindFunc(stmt);
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        callback(stmt);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(DB);
}

int DatabaseManager::prepareSQLStatement(const std::string& sql, sqlite3_stmt*& stmt) {
    int exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0);
    checkPrepareStatement(exit);

    return SQLITE_OK;
}

//some generic checks for all insertData functions, to be inherited
void DatabaseManager::insertDataHelper(const std::string& sql, std::function<void(sqlite3_stmt*)> bindFunc) {
    sqlite3* DB;
    sqlite3_stmt* stmt;

    int exit = sqlite3_open(this->dir, &DB);
    checkOpenDatabase(exit);

    exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0);
    checkPrepareStatement(exit);

    // Call the bind function
    bindFunc(stmt);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(DB) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(DB);
}

//we delete rows based on a unique id
void DatabaseManager::deleteData(int id, const std::string& sql) {
    sqlite3* DB;
    sqlite3_stmt* stmt;

    int exit = sqlite3_open(this->dir, &DB);;
    checkOpenDatabase(exit);

    exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0);
    checkPrepareStatement(exit);

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(DB) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(DB);
}

//the generic checks
void DatabaseManager::checkOpenDatabase(int exit) {
    if (exit != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(DB) << std::endl;
        throw std::runtime_error("Cannot open database");
    }
}

void DatabaseManager::checkPrepareStatement(int exit) {
    if (exit != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << std::endl;
        throw std::runtime_error("Failed to prepare statement");
    }
}