cmake_minimum_required(VERSION 3.26)
project(logJAM)

set(CMAKE_CXX_STANDARD 17)

add_executable(databaseHandler main.cpp
        sqlite3.h
        sqlite3.c
        DatabaseManager.cpp
        DatabaseManager.h
        Leaderboards.cpp
        Leaderboards.h
        MapData.cpp
        MapData.h
)

find_package(SQLite3 REQUIRED)