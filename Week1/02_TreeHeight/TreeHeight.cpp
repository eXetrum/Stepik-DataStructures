#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

int climb_up(map<int, int>& cache, const vector<int>& tree, int v) {
    if (tree[v] == -1) return 1;
    auto it = cache.find(v);
    if (it == cache.end())
        cache[v] = climb_up(cache, tree, tree[v]) + 1;
    return cache[v];
}

int height(const vector<int>& tree) {
    map<int, int> cache;
    int height = 0;

    for (int v = 0; v < tree.size(); ++v) {
        height = max(height, climb_up(cache, tree, v));
    }
    return height;
}

int main() {
    int n;
    cin >> n;
    vector<int> tree(n);
    for (int i = 0; i < n; ++i) cin >> tree[i];
    cout << height(tree);
    return 0;
}