#include <iostream>
#include <string>
#include <map>
//#include "../../include/map.hpp"
using namespace std;

int main() {
    long n, key;
    string cmd, value;
    map<long, string> db;
    
    cin >> n;
    for (int i = 0; i < n; ++i) {
        cin >> cmd;
        if (cmd == "add") {
            // TODO
        } else if(cmd == "find") {
            // TODO
        } else if(cmd == "del") {
            // TODO
        }
        cout << db.size() << endl;
    }
    return 0;
}