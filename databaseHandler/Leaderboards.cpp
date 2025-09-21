//
// Created by  Brandon Thomas on 2024-06-02.
//

#include "Leaderboards.h"

//encapsulation
void Leaderboards::setLeaderboardsDir() {
    DatabaseManager::setDir("../leaderboards.db");
}

//ensures database is always created
Leaderboards::Leaderboards() {
    setLeaderboardsDir();
    sqlite3_open(dir, &DB);
    DatabaseManager::createDB();
}

int Leaderboards::createTable() { //polymorphism, the MapData function is not as simple
    return DatabaseManager::createTable();
}

//sql query, the rest is done in database manager class
std::string Leaderboards::getCreateTableSQL() {
    return "CREATE TABLE IF NOT EXISTS LEADERBOARD("
           "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
           "LEVEL      INT NOT NULL, "
           "SCORE     INT NOT NULL, "
           "PLAYER       TEXT  NOT NULL, "
           "TIME   INT);";
}

//different classback since leaderboards display differently than songs
//provided by sqlite, azColName displays the names of columns, argv displays the i-th column in the row
//eg: id: 5, player: brandon, etc
int Leaderboards::callback(void* NotUsed, int argc, char** argv, char** azColName) {
    for(int i = 0; i < argc; i++) {
        std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << "\n";
    }
    //turnary operator
    std::cout << "\n";
    return 0;
}

void Leaderboards::deleteData(int id) {
    std::string deleteSql = "DELETE FROM LEADERBOARD WHERE ID = ?;";
    DatabaseManager::deleteData(id, deleteSql); //can use same function for both MapData and leaderboards
}

//generic part is in database manager class
void Leaderboards::outputData() {
    sqlite3* DB;
    char* messageError;
    std::string sql = "SELECT * FROM LEADERBOARD;";
    //return all columns of all rows in MapData

    int exit = sqlite3_open(dir, &DB);
    //opens the database

    checkOpenDatabase(exit);
    //we must give it the database, our string, the function that will be used for each output
    //0 as we are only executing one statement, and a way for it to modify the messageError

    exit = sqlite3_exec(DB, sql.c_str(), Leaderboards::callback, 0, &messageError);

    if (exit != SQLITE_OK) {
        //we know this upon exiting
        std::cerr << "Failed to select data: " << sqlite3_errmsg(DB) << std::endl;
        sqlite3_free(messageError);
    }

    sqlite3_close(DB);
}

int Leaderboards::getHiscore(const std::string& player, int level) {
    sqlite3_stmt* stmt;
    int hiscore = 0;
    //default value

    std::string sql = "SELECT MAX(SCORE) FROM LEADERBOARD WHERE PLAYER = ? AND LEVEL = ?;";

    int exit = sqlite3_open(dir, &DB);
    //open the database

   checkOpenDatabase(exit);

    //we must give sql somewhere to store the sqlite statement structure
    //we have to give sqlite out database object, our string in the c version
    // -1 means that the function should read until reaching a null terminator, pz tail is set to 0 if only executing one statement
    exit = sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, 0);

    if (exit != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << std::endl;
        //there is an erorr message function
        return -1;
    }

    //we need to bind 2 values, to find the player's score, for a given level
    sqlite3_bind_text(stmt, 1, player.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, level);

    //after running the sql query, check if it found something. if it doesnt, our int stays 0
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        hiscore = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt); //we're done with the statement so free memory
    sqlite3_close(DB); //close the connection to db

    return hiscore;
}

//generate a leaderboard based on the time, d = within last day, w week, m month. a is set by default
void Leaderboards::genLB(int level, char timeRange) {
    // Build the SQL query based on the time range
    std::string sql = "SELECT PLAYER, MAX(SCORE), MIN(TIME) FROM LEADERBOARD WHERE LEVEL = ? ";
    //switch statements look better than ifstatements
    switch (timeRange) {
        //based on case, append it to the string
        case 'd':
            sql += "AND strftime('%s', 'now') - TIME <= 86400";  // 86400 seconds in a day
            break;
        case 'w':
            sql += "AND strftime('%s', 'now') - TIME <= 604800";  // 604800 seconds in a week
            break;
        case 'm':
            sql += "AND strftime('%s', 'now') - TIME <= 2629800";  // 2629800 seconds in a month (approx.)
            break;
        case 'a':
            // No additional if its clause for all time
            break;
        default:
            std::cerr << "Invalid time range: " << timeRange << std::endl;
            return;
    }
    sql += " GROUP BY PLAYER ORDER BY MAX(SCORE) DESC, MIN(TIME) ASC;"; //end of the statement

    // Define the bind function
    //auto data type since its complex
    auto bindFunc = [level](sqlite3_stmt* stmt) {
        sqlite3_bind_int(stmt, 1, level);
    };

    // Define the callback function
    //auto data type since its complex
    auto callback = [](sqlite3_stmt* stmt) {
        std::string player = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        int score = sqlite3_column_int(stmt, 1);

        std::cout << "Score: " << score << ", Player: " << player << "\n";
    };

    // Execute the query and output the leaderboard
    executeSQLWithCallback(sql, bindFunc, callback);
}

void Leaderboards::insertData(int level, int score, const std::string& player) {
    std::string sql = "INSERT INTO LEADERBOARD (LEVEL, SCORE, PLAYER, TIME) VALUES (?, ?, ?, ?);";
    auto bindFunc = [level, score, player](sqlite3_stmt* stmt) {
        sqlite3_bind_int(stmt, 1, level);
        sqlite3_bind_int(stmt, 2, score);
        sqlite3_bind_text(stmt, 3, player.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 4, std::time(0)); // Unix timestamp
        //we bind this to the '?' in the statement
    };
    insertDataHelper(sql, bindFunc);
}