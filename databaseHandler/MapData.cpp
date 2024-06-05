//
// Created by  Brandon Thomas on 2024-06-03.
//

#include "MapData.h"

void MapData::setMapDataDir() {
    DatabaseManager::setDir("../map-data.db");
}

MapData::MapData() {
    setMapDataDir();
    sqlite3_open(dir, &DB);
    DatabaseManager::createDB();
    currentLevelId = getMinId(); // Initialize currentLevelId to the lowest ID
    loadCurrentValue();
}

MapData::~MapData() {
    // Destructor
}

int MapData::createTable() {
    return DatabaseManager::createTable();
}

std::string MapData::getCreateTableSQL() {
    return "CREATE TABLE IF NOT EXISTS MapData("
           "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
           "SongTitle TEXT NOT NULL,"
           "SongArtist TEXT NOT NULL,"
           "Length INTEGER NOT NULL,"
           "BPM INTEGER NOT NULL,"
           "Difficulty INTEGER NOT NULL,"
           "Source TEXT NOT NULL);";
}

void MapData::insertData(const std::string& songTitle, const std::string& songArtist, int length, int bpm, int difficulty, const std::string& source) {
    std::string sql = "INSERT INTO MapData(SongTitle, SongArtist, Length, BPM, Difficulty, Source) VALUES (?, ?, ?, ?, ?, ?);";
    insertDataHelper(sql, [songTitle, songArtist, length, bpm, difficulty, source](sqlite3_stmt* stmt) {
        sqlite3_bind_text(stmt, 1, songTitle.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, songArtist.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 3, length);
        sqlite3_bind_int(stmt, 4, bpm);
        sqlite3_bind_int(stmt, 5, difficulty);
        sqlite3_bind_text(stmt, 6, source.c_str(), -1, SQLITE_STATIC);
    });
}

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
    currentLevelId+=9;
    nextLv();
}

void MapData::displayLevel() {
    std::string sql = "SELECT * FROM MapData WHERE ID = ?;";
    sqlite3_stmt* stmt;
    bool idExists = false;

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
        callback(nullptr, colCount, argv, azColName);
        delete[] argv;
        delete[] azColName;
        currentLevelId = id;  // Update currentLevelId to the valid ID
        sqlite3_finalize(stmt);
        return true;
    }
    sqlite3_finalize(stmt);
    return false;
}
int MapData::getMaxId() {
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

void MapData::loadCurrentValue() {
    std::string sql = "SELECT Value FROM CurrentValue WHERE ID = 1;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            currentLevelId = sqlite3_column_int(stmt, 0);
        } else {
            std::cout << "Failed to load current value: " << sqlite3_errmsg(DB) << std::endl;
        }
        sqlite3_finalize(stmt);
    } else {
        std::cout << "Failed to prepare statement: " << sqlite3_errmsg(DB) << std::endl;
    }
}