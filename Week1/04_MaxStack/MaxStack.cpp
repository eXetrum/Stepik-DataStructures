#include <iostream>
#include <string>
#include <stack>
using namespace std;

template <class Type>
class MaxStack {
	stack<Type> values;
	stack<Type> max;
public:
	void push(const Type& item) {
		values.push(item);
		Type max_item = item;
		if (max.empty() || item > max.top())
			max.push(item);
	}
};

int main() {
	string cmd;
	int n;
	
	cin >> n;
	for (int i = 0; i < n; ++i) {
		cin >> cmd;
		if (cmd == "push") {
			int value;
			cin >> value;
		} else if(cmd == "max") {
		} else if(cmd == "pop") {
		}
	}
	return 0;
}