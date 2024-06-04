//
// Created by  Brandon Thomas on 2024-06-03.
//

#include "MapData.h"

MapData::MapData() {
    // Other initialization code...
}

MapData::~MapData() {
    // Destructor
}

int MapData::createTable() {
    return DatabaseManager::createTable();
}

std::string MapData::getCreateTableSQL() {
    return "CREATE TABLE IF NOT EXISTS MapData("
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