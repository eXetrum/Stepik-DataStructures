#include <iostream>
#include <vector>
using namespace std;

class disjoint_set {
protected:
    vector<int> parent;
    vector<int> rank;
private:
    void make_set(int i) {
        parent[i] = i;
        rank[i] = 0;
    }
public:
    disjoint_set(int max_value) { 
        parent.resize(max_value + 1); 
        rank.resize(max_value + 1);
        for (int i = 0; i < max_value; ++i) {
            make_set(i);
        }
    }

    int find(int i) const {
        while (i != parent[i]) {
            i = parent[i];
        }
        return i;
    }

    void union_set(int i, int j) {
        int i_id = find(i);
        int j_id = find(j);
        
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
    int n, m;

    return 0;
}