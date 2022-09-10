#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class disjoint_set {
protected:
    vector<long> parent;
    vector<long> rank;
public:
    disjoint_set(long max_value) {
        parent.resize(max_value + 1);
        rank.resize(max_value + 1);
        for (long i = 1; i <= max_value; ++i) {
            make_set(i);
        }
    }

    void make_set(long i) {
        parent[i] = i;
        rank[i] = 0;
    }

    long find(long i) {
        // Path compression
        if (i != parent[i]) {
            parent[i] = find(parent[i]);
        }
        return parent[i];
    }

    void union_set(int i, int j) {
        long i_id = find(i);
        long j_id = find(j);

        if (i_id == j_id) return;

        if (rank[i_id] > rank[j_id]) {
            parent[j_id] = i_id;
        } else {
            parent[i_id] = j_id;
            if (rank[i_id] == rank[j_id]) { ++rank[j_id]; }
        }

    }
};

int main() {
    long n, e, d;
    bool result = true;

    cin >> n >> e >> d;
    disjoint_set S(n);

    for (long i = 1; i <= n; ++i) { S.make_set(i); }

    // Equality
    for (long i = 0; i < e; ++i) {
        long dst, src;
        cin >> dst >> src;
        S.union_set(dst, src);
    }

    // Inequality
    for (long i = 0; i < d && result; ++i) {
        long dst, src;
        cin >> dst >> src;
        result = S.find(dst) != S.find(src);
    }

    cout << result;
    return 0;
}