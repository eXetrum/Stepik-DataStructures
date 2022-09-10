#include <iostream>
#include <string>
#include <map>
using namespace std;

int main() {
    long n, key;
    string cmd, value;
    map<long, string> db;
    
    cin >> n;
    for (int i = 0; i < n; ++i) {
        cin >> cmd;
        if (cmd == "add") {
            cin >> key >> value;
            db[key] = value;
        } else if(cmd == "find") {
            cin >> key;
            if (db.find(key) != db.end()) { cout << db[key] << endl; }
            else { cout << "not found" << endl; }
        } else if(cmd == "del") {
            cin >> key;
            db.erase(key);
        }
        //db.dump();
    }
    return 0;
}