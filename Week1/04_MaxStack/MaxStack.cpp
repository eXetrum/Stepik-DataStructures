#include <iostream>
#include <string>
#include <stack>
using namespace std;

template <class Type=int>
class MaxStack {
	stack<Type> values;
	stack<Type> cur_max;
public:
	void push(const Type& item) {
		values.push(item);
		if (cur_max.empty() || item > cur_max.top())
			cur_max.push(item);
		else
			cur_max.push(cur_max.top());
	}
	
	Type max() const { return cur_max.top(); }
	
	void pop() {
		values.pop();
		cur_max.pop();
	}
};

int main() {
	string cmd;
	int n;
	MaxStack<> st;
	
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