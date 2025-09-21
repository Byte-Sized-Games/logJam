#include "DatabaseManager.h"

using namespace std;

const char* DatabaseManager::dir = "../leaderboards.db";

DatabaseManager::DatabaseManager() {
    sqlite3_open(dir, &DB);
}

//in mapData and leaderboards
int DatabaseManager::createDB() {
    sqlite3* DB;
    int exit = 0;
    sqlite3_open(dir, &DB);
    sqlite3_close(DB);
    return 0;
}

//called externally
void DatabaseManager::setDir(const char* directory) {
    dir = directory;
}

//theres a lot that can be shared between leaderboards and mapdata, so use inheritance
//its just the sql query thats different

//here, the tables are always created, this is called from MapData and Leaderboards
//in constructor of MapData class and Leaderboards
int DatabaseManager::createTable() {
    sqlite3 *DB;
    char *messageError;
    std::string sql = getCreateTableSQL();
    try {
        int exit = sqlite3_open(this->dir, &DB);  // Open the database
        checkOpenDatabase(exit);

        //we must give sql somewhere to store the sqlite statement structure
        //we have to give sqlite out database object, our string in the c version
        // -1 means that the function should read until reaching a null terminator, pz tail is set to 0 if only executing one statement
        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
        if (exit != SQLITE_OK) {
            std::cerr << "createTable function failed." << std::endl;
            sqlite3_free(messageError);
        }

        //this is used as currentValue is useful for both Leaderboards and MapData
        sql = "CREATE TABLE IF NOT EXISTS CurrentValue (ID INTEGER PRIMARY KEY, Value INTEGER);";
        sqlite3_stmt* stmt;
        exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0);
        checkPrepareStatement(exit);
        sqlite3_step(stmt); //evaluate statement
        sqlite3_finalize(stmt); //terminate
        sql = "INSERT OR IGNORE INTO CurrentValue (ID, Value) VALUES (1, 0);";
        exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0);
        checkPrepareStatement(exit);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        sqlite3_close(DB);
    }
    catch (const exception &e) {
        cerr << e.what(); //output exceptions and dont break down
    }
    return 0;
}

//it has to do checks and generate a statement since we're inserting variables
//called to combine the callback and execution
void DatabaseManager::executeSQLWithCallback(const std::string& sql, std::function<void(sqlite3_stmt*)> bindFunc, std::function<void(sqlite3_stmt*)> callback) {
    sqlite3* DB; //so it knows the file path
    sqlite3_stmt* stmt; //give sql somewhere to store our statement structure

    int exit = sqlite3_open(this->dir, &DB);
    checkOpenDatabase(exit);

    //we must give sql somewhere to store the sqlite statement structure
    //we have to give sqlite out database object, our string in the c version
    // -1 means that the function should read until reaching a null terminator, pz tail is set to 0 if only executing one statement
    exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0);
    checkPrepareStatement(exit);

    // Call the bind function, if provided, so we can properly output
    if (bindFunc) {
        bindFunc(stmt);
    }

    //we output all rows, SQLITE_ROW is a constant
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        callback(stmt);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(DB);
}

//reference to our sql string, reference to statement
//these few lines of code were repeated a lot
//called in MapData classes to get the min and max ID
int DatabaseManager::prepareSQLStatement(const std::string& sql, sqlite3_stmt*& stmt) {
    //we must give sql somewhere to store the sqlite statement structure
    //we have to give sqlite out database object, our string in the c version
    // -1 means that the function should read until reaching a null terminator, pz tail is set to 0 if only executing one statement
    int exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0);
    checkPrepareStatement(exit);

    return SQLITE_OK;
}

//some generic checks for all insertData functions, to be inherited
//reference to string and callable object, is needed to insert the data
//called from MapData and Leaderboards after insertData
void DatabaseManager::insertDataHelper(const std::string& sql, std::function<void(sqlite3_stmt*)> bindFunc) {
    sqlite3* DB;
    sqlite3_stmt* stmt;

    int exit = sqlite3_open(this->dir, &DB);
    checkOpenDatabase(exit);

    //we must give sql somewhere to store the sqlite statement structure
    //we have to give sqlite out database object, our string in the c version
    // -1 means that the function should read until reaching a null terminator, pz tail is set to 0 if only executing one statement

    exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0);
    checkPrepareStatement(exit);

    // Call the bind function
    bindFunc(stmt);

    //evaluate statement
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(DB) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(DB);
}

//we delete rows based on a unique id
//called in mapData and leaderboards, polymorphism
void DatabaseManager::deleteData(int id, const std::string& sql) {
    sqlite3* DB;
    sqlite3_stmt* stmt;

    //get the directory, it changes based on the class
    int exit = sqlite3_open(this->dir, &DB);;
    checkOpenDatabase(exit);

    //we must give sql somewhere to store the sqlite statement structure
    //we have to give sqlite out database object, our string in the c version
    // -1 means that the function should read until reaching a null terminator, pz tail is set to 0 if only executing one statement

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
//called in various functions before executing statements to make sure nothing breaks
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