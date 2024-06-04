#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <sqlite3.h>
#include <string>
#include <functional>
#include <iostream>
#include <cstdio>
#include <sqlite3.h>
#include <string>
#include <chrono>

class DatabaseManager {
public:
    DatabaseManager();
    ~DatabaseManager();
    static int createDB(); //can be statis since it doesnt need to do polymorphism
    //all the virtual functions have a generic part and then are overridden with a specific part
    virtual int createTable();
    //callback had to be static so cant be here
    virtual void deleteData(int id, const std::string& sql);
    //outputdata does not have enough generic parts for this
    void insertDataHelper(const std::string &sql, std::function<void(sqlite3_stmt *)> bindFunc);
    int executeSQL(const std::string& sql, std::function<void(sqlite3_stmt*)> bindFunc = nullptr);
    virtual int prepareSQLStatement(const std::string& sql, sqlite3_stmt*& stmt);
    virtual std::string getCreateTableSQL() = 0;
    //these two generic things check if things are working and display an error message if not
    void checkOpenDatabase(int exit);
    void checkPrepareStatement(int exit);
    static void setDir(const char* directory);


protected:
    //im doing encapsulation ooh aah
    const char* s;
    sqlite3* DB;
    static const char* dir;
};

#endif //DATABASEMANAGER_H
