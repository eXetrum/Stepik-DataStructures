#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class disjoint_set {
protected:
    vector<pair<long, long>> parent;
    vector<long> rank;
    long max_table_len;
private:
    
public:
    disjoint_set(long max_value)
        : max_table_len(0) {
        parent.resize(max_value + 1); 
        rank.resize(max_value + 1);
        for (int i = 1; i <= max_value; ++i) {
            make_set(i, 0);
        }
    }
    
    void make_set(long i, long size) {
        parent[i].first = i;
        parent[i].second = size;
        rank[i] = 0;
        
        max_table_len = max(max_table_len, size);
    }

    pair<long, long> find(long i) {
        // Path compression
        if (i != parent[i].first) {
            auto it = find(parent[i].first);
            parent[i].first = it.first;
            parent[i].second = max(parent[i].second, it.second);
        }
        return parent[i];
    }

    int get_max() const { return max_table_len; }

    void union_set(int i, int j) {
        pair<long, long> i_id = find(i);
        pair<long, long> j_id = find(j);
        
        if (i_id.first == j_id.first) return;

        if (rank[i_id.first] > rank[j_id.first]) {
            parent[j_id.first].first = i_id.first;
            parent[j_id.first].second += i_id.second;
            
            parent[i_id.first].second = parent[j_id.first].second;
            max_table_len = max(max_table_len, parent[j_id.first].second);
        } else {
            parent[i_id.first].first = j_id.first;
            parent[i_id.first].second += j_id.second;

            parent[j_id.first].second = parent[i_id.first].second;
            max_table_len = max(max_table_len, parent[i_id.first].second);
            if (rank[i_id.first] == rank[j_id.first]) { ++rank[j_id.first]; }
        }

    }
    void dump() {
        for (int i = 0; i < parent.size(); ++i) {
            cout << parent[i].first << " " << parent[i].second << endl;
        }
        cout << "-------" << endl;
    }
};

int main() {
    long n, m, r;
    vector<long> R;
    cin >> n >> m;
    disjoint_set S(n);

    for (long i = 1; i <= n; ++i) {
        cin >> r;
        S.make_set(i, r);
        //S.dump();
    }

    for (long i = 0; i < m; ++i) {
        long dst, src;
        cin >> dst >> src;
        S.union_set(dst, src);
        cout << S.get_max() << endl;
        //S.dump();
    }


    return 0;
}