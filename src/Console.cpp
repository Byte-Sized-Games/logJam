//
// Created by phoenix on 15/04/24.
//
#include "string"
#include "iostream"

using namespace std;

namespace console {
    bool level;
    void log(const string& target) {
        cout << target << endl;
    }
    void debug(const string& target) {
        if(level) log(target);
    }
}
