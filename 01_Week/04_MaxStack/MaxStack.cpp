#include <iostream>
#include <string>
using namespace std;
#include "..\..\include\max_stack.hpp"

int main() {
	string cmd;
	int n;
	max_stack<> st;
	
	cin >> n;
	for (int i = 0; i < n; ++i) {
		cin >> cmd;
		if (cmd == "push") {
			int value;
			cin >> value;
			st.push(value);
		} else if(cmd == "max") {
			cout << st.max() << endl;
		} else if(cmd == "pop") {
			st.pop();
		}
	}
	return 0;
}