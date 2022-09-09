#include <iostream>
#include <queue>
using namespace std;

struct processor_t {
    long id;
    long t;
    processor_t(int id)
        : id(id), t(0) { }

    bool operator<(const processor_t& rhs) const { 
        if (t == rhs.t)
            return id > rhs.id;
        return t > rhs.t; 
    }
};

int main() {
    long n, m;
    priority_queue<processor_t> pq;
    cin >> n >> m;
    for (long i = 0; i < n; ++i) {
        pq.push(processor_t(i));
    }
    for (long i = 0; i < m; ++i) {
        long t;
        cin >> t;
        processor_t p = pq.top();
        pq.pop();
        cout << p.id << " " << p.t << endl;
        p.t += t;
        pq.push(p);
    }
    
    return 0;
}