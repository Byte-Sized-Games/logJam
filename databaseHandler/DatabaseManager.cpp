#include "DatabaseManager.h"

using namespace std;

const char* DatabaseManager::dir = "../leaderboards.db";

DatabaseManager::DatabaseManager() {
    sqlite3_open(dir, &DB);
}

DatabaseManager::~DatabaseManager() {
    sqlite3_close(DB);
}

int DatabaseManager::createDB() {
    cout << "pain and suffering 1\n";
    sqlite3* DB;
    int exit = 0;
    cout << "pain and suffering 2\n";
    sqlite3_open(dir, &DB);
    cout << "pain and suffering 3\n";
    sqlite3_close(DB);
    cout << "pain and suffering 4\n";
    return 0;
}

void DatabaseManager::setDir(const char* directory) {
    dir = directory;
}

int DatabaseManager::createTable() {
    sqlite3 *DB;
    char *messageError;
    std::cout << "pain and suffering 7\n";
    std::string sql = getCreateTableSQL();
    std::cout << "pain and suffering 8\n";
    try {
        int exit = 0;
        std::cout << "pain and suffering 9\n";

        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
        if (exit != SQLITE_OK) {
            cerr << "createTable function failed." << endl;
            sqlite3_free(messageError);
        }

        sql = "CREATE TABLE IF NOT EXISTS CurrentValue (ID INTEGER PRIMARY KEY, Value INTEGER);";
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }

        sql = "INSERT OR IGNORE INTO CurrentValue (ID, Value) VALUES (1, 0);";
        if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
        sqlite3_close(DB);
    }
    catch (const exception &e) {
        cerr << e.what();
    }

    return 0;
}

int DatabaseManager::executeSQL(const std::string& sql, std::function<void(sqlite3_stmt*)> bindFunc) {
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

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(DB) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(DB);

    return exit;
}

int DatabaseManager::prepareSQLStatement(const std::string& sql, sqlite3_stmt*& stmt) {
    int exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0);
    checkPrepareStatement(exit);

    return SQLITE_OK;
}


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