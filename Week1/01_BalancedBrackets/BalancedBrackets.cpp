#include <iostream>
#include <stack>
#include <string>
#include <algorithm>
using namespace std;

int is_balanced(const string& s) {
    stack<pair<char, int>> st;
    for (int i = 0; i < s.size(); ++i) {
        char c = s[i];
        // Open bracket
        if (c == '[' || c == '(' || c == '{')
            st.push(make_pair(c, i + 1));
        // Close bracket
        else if (c == ']' || c == ')' || c == '}') {
            if (st.empty()) return i + 1;
            auto p = st.top(); st.pop();
            char top = p.first;
            int pos = p.second;
            if ((top == '[' && c != ']') || (top == '(' && c != ')') || (top == '{' && c != '}')) return i + 1;
        }
    }

    if (!st.empty()) {
        auto p = st.top();
        return p.second;
    }
    return -1;
}


int main() {
    string s;
    cin >> s;
    int pos;
    if ((pos = is_balanced(s)) == -1) { cout << "Success"; }
    else { cout << pos; }
    return 0;
}