#include <iostream>
#include <string>
#include <vector>
#include <list>
using namespace std;

template <class key_t=string>
class hash_table {
    vector<list<key_t>> bucket;
public:
    hash_table(size_t size) { bucket.resize(size); }

    void add(const key_t& key) {
        // TODO
    }

    bool find(const key_t& key) const {
        // TODO
    }

    void check(size_t i) const {
        // TODO
    }

    void remove(const key_t& key) {
        // TODO
    }
};

int main() {
    int m, n, c;
    string cmd, key;

    cin >> m >> n;
    
    hash_table<string> table(m);

    for(int i = 0; i < n; ++i) {
        cin >> cmd;
        if (cmd == "add") {
            cin >> key;
            table.add(key);
        } else if(cmd == "del") {
            cin >> key;
            table.remove(key);
        } else if(cmd == "find") {
            cin >> key;
            cout << (table.find(key) ? "yes" : "no") << endl;
        } else if(cmd == "check") {
            cin >> c;
            table.check(c);
        }
    }
    


    return 0;
}