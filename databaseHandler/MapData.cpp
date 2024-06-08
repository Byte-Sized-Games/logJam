//
// Created by  Brandon Thomas on 2024-06-03.
//

#include "MapData.h"

//encapsulation
void MapData::setMapDataDir() {
    DatabaseManager::setDir("../map-data.db");
}

//this ensures the database is always created
MapData::MapData() {
    setMapDataDir();
    sqlite3_open(dir, &DB);
    DatabaseManager::createDB();
    loadCurrentValue();
}

//sql query that creates 2 tables. the first one is through createTable as there are reusable portions of the leaderboard table
//the other table is to store the currentvalue
int MapData::createTable() {
    int exit = DatabaseManager::createTable();

    std::string sql = "CREATE TABLE IF NOT EXISTS CurrentValue (ID INTEGER PRIMARY KEY, Value INTEGER);";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    } else {
        return -1;
    }

    sql = "INSERT OR IGNORE INTO CurrentValue (ID, Value) VALUES (1, 0);";
    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    } else {
        return -1;
    }

    return exit;
}


//sql query is used in databasemanager
std::string MapData::getCreateTableSQL() {
    return "CREATE TABLE IF NOT EXISTS MapData("
           "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
           "Title TEXT NOT NULL,"
           "Artist TEXT NOT NULL,"
           "Length INTEGER NOT NULL,"
           "BPM INTEGER NOT NULL,"
           "Difficulty INTEGER NOT NULL,"
           "Level INTEGER NOT NULL,"
           "Source TEXT NOT NULL);";
}

//insert a song entry
void MapData::insertData(const std::string& songTitle, const std::string& songArtist,
                         int length, int bpm, int difficulty, int level, const std::string& source) {
    // Check for duplicates before inserting a new song
    if (!isDuplicate(level)) {
        std::string sql = "INSERT INTO MapData(Title, Artist, Length, BPM, Difficulty, Level, Source) VALUES (?, ?, ?, ?, ?, ?, ?);"; // Updated SQL statement
        insertDataHelper(sql, [songTitle, songArtist, length, bpm, difficulty, level, source](sqlite3_stmt* stmt) {
            sqlite3_bind_text(stmt, 1, songTitle.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, songArtist.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_int(stmt, 3, length);
            sqlite3_bind_int(stmt, 4, bpm);
            sqlite3_bind_int(stmt, 5, difficulty);
            sqlite3_bind_int(stmt, 6, level); // Bind level
            sqlite3_bind_text(stmt, 7, source.c_str(), -1, SQLITE_STATIC);
        });
    } else {
        std::cout << "Level already exists" << std::endl;
    }
}

//output all data, helpful for debugging
void MapData::outputData() {
    sqlite3* DB;
    char* messageError;
    std::string sql = "SELECT * FROM MapData;";

    int exit = sqlite3_open(dir, &DB);

    checkOpenDatabase(exit);

    exit = sqlite3_exec(DB, sql.c_str(), MapData::callback, 0, &messageError);

    if (exit != SQLITE_OK) {
        std::cerr << "Failed to select data: " << sqlite3_errmsg(DB) << std::endl;
        sqlite3_free(messageError);
    }

    sqlite3_close(DB);
}

//used to print the data, outputData selects the data
int MapData::callback(void* NotUsed, int argc, char** argv, char** azColName) {
    for(int i = 0; i < argc; i++) {
        std::cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << "\n";
    }
    std::cout << "\n";
    return 0;
}

void MapData::deleteData(int id) {
    std::string deleteSql = "DELETE FROM MapData WHERE ID = ?;";
    DatabaseManager::deleteData(id, deleteSql);
}

//go through next and previous levels in the table
void MapData::nextLv() {

    currentLevelId++;
    if (currentLevelId > getMaxId()) {
        currentLevelId = getMinId();
    }
    displayLevel();
    saveCurrentValue();
}

void MapData::next10Lv() {
    currentLevelId+=9;
    nextLv();
}

void MapData::prevLv() {
    currentLevelId--;
    if (currentLevelId < getMinId()) {
        currentLevelId = getMaxId();
    }
    displayLevel();
    saveCurrentValue();
}

void MapData::prev10Lv() {
    currentLevelId-=9;
    prevLv();
}

//display info on the current level, make sure it exists though
//if it doesnt exist display the first level
void MapData::displayLevel() {
    std::string sql = "SELECT * FROM MapData WHERE ID = ?;";
    sqlite3_stmt* stmt;
    bool idExists = false;

    if (currentLevelId < getMinId() || currentLevelId > getMaxId()){
        currentLevelId = getMinId(); }

    // Try IDs from currentLevelId to max ID
    for (int id = currentLevelId; id <= getMaxId() && !idExists; id++) {
        idExists = tryDisplayLevel(id, sql, stmt);
    }

    // If no valid ID found, try IDs from min ID to currentLevelId
    for (int id = getMinId(); id < currentLevelId && !idExists; id++) {
        idExists = tryDisplayLevel(id, sql, stmt);
    }

    if (!idExists) {
        std::cout << "No valid level ID found\n";
    }
}

//to make sure an error doesnt occur when sqlite tries to display a level that doesnt exist
bool MapData::tryDisplayLevel(int id, const std::string& sql, sqlite3_stmt*& stmt) {
    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0) != SQLITE_OK) {
        std::cout << "Failed to prepare statement\n";
        return false;
    }
    sqlite3_bind_int(stmt, 1, id);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int colCount = sqlite3_column_count(stmt);
        char** argv = new char*[colCount];
        char** azColName = new char*[colCount];
        for (int i = 0; i < colCount; i++) {
            argv[i] = (char*)sqlite3_column_text(stmt, i);
            azColName[i] = (char*)sqlite3_column_name(stmt, i);
            if (argv[i] == nullptr || azColName[i] == nullptr) {
                std::cout << "Failed to get column text or name\n";
                delete[] argv;
                delete[] azColName;
                sqlite3_finalize(stmt);
                return false;
            }
        }
        displayLevelCallback(nullptr, colCount, argv, azColName); // Use displayLevelCallback here
        delete[] argv;
        delete[] azColName;
        currentLevelId = id;  // Update currentLevelId to the valid ID
        sqlite3_finalize(stmt);
        return true;
    }
    sqlite3_finalize(stmt);
    return false;
}

//used in next and previous functions
int MapData::getMaxId() {
    DatabaseManager::checkOpenDatabase(sqlite3_open(this->dir, &DB));
    std::string sql = "SELECT MAX(ID) FROM MapData;";
    sqlite3_stmt* stmt;
    prepareSQLStatement(sql, stmt);
    int maxId = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        maxId = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return maxId;
}

int MapData::getMinId() {
    DatabaseManager::checkOpenDatabase(sqlite3_open(this->dir, &DB));
    std::string sql = "SELECT MIN(ID) FROM MapData;";
    sqlite3_stmt* stmt;
    prepareSQLStatement(sql, stmt);
    int minId = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        minId = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return minId;
}

//save it to the tables
void MapData::saveCurrentValue() {
    std::string sql = "UPDATE CurrentValue SET Value = ? WHERE ID = 1;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, currentLevelId);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cout << "Failed to save current value: " << sqlite3_errmsg(DB) << std::endl;
        }
        sqlite3_finalize(stmt);
    } else {
        std::cout << "Failed to prepare statement: " << sqlite3_errmsg(DB) << std::endl;
    }
}

//has to be gotten before its used
void MapData::loadCurrentValue() {
    std::string sql = "SELECT Value FROM CurrentValue WHERE ID = 1;";
    sqlite3_stmt* stmt;
    int exit = sqlite3_open(this->dir, &DB);
    if (exit != SQLITE_OK) {
        return;
    }
    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            currentLevelId = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }
    sqlite3_close(DB);
}

//a different callback for displayLevel, we dont want it to display certain columns in game
int MapData::displayLevelCallback(void* NotUsed, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        std::string columnName = azColName[i];
        if (columnName != "ID" && columnName != "Source" && columnName != "Level") {
            std::cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << "\n";
        }
    }
    std::cout << "\n";
    return 0;
}

int MapData::getCurrentId() const {
    return currentLevelId;
}

//gets the currentLevel from the table
int MapData::getCurrentLevel() {
    std::string sql = "SELECT Level FROM MapData WHERE ID = ?;";
    sqlite3_stmt* stmt;
    int level = -1;  // Initialize level to -1 to indicate an error if no level is found

    int exit = sqlite3_open(this->dir, &DB);
    checkOpenDatabase(exit);

    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, currentLevelId);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            level = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    } else {
        std::cout << "Failed to prepare statement: " << sqlite3_errmsg(DB) << std::endl;
    }

    sqlite3_close(DB);
    return level;
}

//to make sure that multiple songs for the same level cant be created, as leaderboards are per level
bool MapData::isDuplicate(int level) {
    sqlite3* DB;
    sqlite3_stmt* stmt;
    int exit = sqlite3_open(this->dir, &DB);
    checkOpenDatabase(exit);

    std::string sql = "SELECT * FROM MapData WHERE Level = ?;";
    exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0);
    checkPrepareStatement(exit);

    sqlite3_bind_int(stmt, 1, level);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        sqlite3_finalize(stmt);
        sqlite3_close(DB);
        return true;  // A duplicate was found
    }

    sqlite3_finalize(stmt);
    sqlite3_close(DB);
    return false;  // No duplicate was found
}