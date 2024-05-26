#include<iostream>
#include <cstdio>
#include <sqlite3.h>

using namespace std;


static int createDB(const char* s)
{
    sqlite3* DB;
    int exit = 0;

    exit = sqlite3_open(s, &DB);

    sqlite3_close(DB);

    return 0;
}

static int createTable(const char* s) {
    sqlite3 *DB;
    char *messageError;

    string sql = "CREATE TABLE IF NOT EXISTS LEADERBOARD("
                 "ID INTEGER PRIMARY KEY AUTOINCREMENT, " //no touchy, this is the primary key
                 "LEVEL      INT NOT NULL, "
                 "SCORE     INT NOT NULL, "
                 "PLAYER       TEXT  NOT NULL, "
                 "TIME   INT, ";

    try {
        int exit = 0;
        exit = sqlite3_open(s, &DB);
        // An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here
        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
        if (exit != SQLITE_OK) {
            cerr << "createTable function failed." << endl;
            sqlite3_free(messageError);
        } else
            cout << "Table created Successfully" << endl;
        sqlite3_close(DB);
    }
    catch (const exception &e) {
        cerr << e.what();
    }

    return 0;
}


int main()
{
    const char* dir = "../leaderboards.db";

    sqlite3* DB;

    //where leaderboards will be stored
    createDB(dir);
    createTable(dir);



    return 0;
}

