#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
using namespace std;

class hash_table {
    size_t m;
    vector<list<string>> bucket;
private:
    long long hash(const string& S) const {
        const long long X = 263;
        const long long P = 1000000007;
        long long x = 1;

        long long sum = 0;
        for (size_t i = 0; i < S.size(); ++i) {
            sum += (S[i] * x);
            x = (x * X) % P;
        }

        return (sum % P) % m;
    }
public:
    hash_table(long long m)
        : m(m) {
        bucket.resize(m);
    }

    void add(const string& key) {
        long long idx = hash(key);
        auto it = std::find(bucket[idx].begin(), bucket[idx].end(), key);
        if (it != bucket[idx].end()) return;

        bucket[idx].push_front(key);
    }

    bool find(const string& key) const {
        long long idx = hash(key);
        return std::find(bucket[idx].begin(), bucket[idx].end(), key) != bucket[idx].end();
    }

    void check(long long idx) const {
        if (bucket[idx].empty()) {
            cout << endl;
            return;
        }
        for (auto it : bucket[idx]) {
            cout << it << " ";
        }
        cout << endl;
    }

    void remove(const string& key) {
        long long idx = hash(key);
        auto it = std::find(bucket[idx].begin(), bucket[idx].end(), key);
        if (it == bucket[idx].end()) return;
        bucket[idx].erase(it);
    }
};

int main() {
    long long  m, n, c;
    string cmd, key;

    cin >> m >> n;

    hash_table table(m);

    for (unsigned long long i = 0; i < n; ++i) {
        cin >> cmd;
        if (cmd == "add") {
            cin >> key;
            table.add(key);
        }
        else if (cmd == "del") {
            cin >> key;
            table.remove(key);
        }
        else if (cmd == "find") {
            cin >> key;
            cout << (table.find(key) ? "yes" : "no") << endl;
        }
        else if (cmd == "check") {
            cin >> c;
            table.check(c);
        }
    }
    return 0;
}