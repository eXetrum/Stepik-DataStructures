#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;


template <class Type = int, class Comparator = std::less<Type> >
class BuildHeapProblem {
    vector<pair<int, int>> swaps;
    Comparator cmp;
private:

    int parent(int i) { return (i + 1) / 2 - 1; }
    int left(int i) { return 2 * i + 1; }
    int right(int i) { return 2 * i + 2; }


    void sift_down(vector<Type>& A, int index) {
        int L_child, R_child;

        while ((L_child = left(index)) < A.size()) {
            int lowest_idx = L_child;
            if ((R_child = right(index)) < A.size() && cmp(A[R_child], A[L_child])) {
                lowest_idx = R_child;
            }

            if (!cmp(A[lowest_idx], A[index])) break;

            std::swap(A[index], A[lowest_idx]);
            
            swaps.push_back(make_pair(index, lowest_idx));

            index = lowest_idx;
        }
    }

    void build_heap(vector<Type>& A) {
        int n = (int)A.size();

        for (int i = n / 2 - 1; i >= 0; --i) {
            sift_down(A, i);
        }
    }
public:
    void solve(vector<Type>& A) {
        swaps.clear();
        build_heap(A);
    }

    const vector<pair<int, int>>& get_swaps() const { return swaps; }
};

int main() {
    int n;
    cin >> n;
    vector<int> v(n);
    for (int i = 0; i < n; ++i) {
        cin >> v[i];
    }
    BuildHeapProblem<> problem;
    problem.solve(v);
    auto S = problem.get_swaps();

    cout << S.size() << endl;
    for (auto it : S) {
        cout << it.first << " " << it.second << endl;
    }

    return 0;
}