#include <iostream>
#include <string>
#include <vector>
#include <list>
using namespace std;

class hash_table {
    size_t m;
    vector<list<string>> bucket;
private:
    size_t hash(const string& S) const {
        const unsigned long long X = 263;
        const unsigned long long P = 1000000007;
        size_t x = 1;

        unsigned long long sum = 0;
        for (size_t i = 0; i < S.size(); ++i) {
            sum += ((S[i] * x) % P);
        }

        return sum % m;
    }
public:
    hash_table(size_t m)
        : m(m) { bucket.resize(m); }

    void add(const string& key) {
        // TODO
    }

    bool find(const string& key) const {
        // TODO
    }

    void check(size_t i) const {
        // TODO
    }

    void remove(const string& key) {
        // TODO
    }
};

int main() {
    int m, n, c;
    string cmd, key;

    cin >> m >> n;
    
    hash_table table(m);

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