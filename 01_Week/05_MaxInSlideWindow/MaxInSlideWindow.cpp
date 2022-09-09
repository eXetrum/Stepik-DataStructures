#include <iostream>
#include <queue>
#include <stack>
#include <stdexcept>
using namespace std;

template <class Type = int>
class max_stack {
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

	bool empty() const { return values.empty(); }

	Type top() const { 
		if (empty()) throw runtime_error("stack is empty");
		return values.top(); 
	}

	Type max() const { 
		if (empty()) throw runtime_error("stack is empty");
		return cur_max.top(); 
	}

	void pop() {
		if (empty()) throw runtime_error("stack is empty");
		values.pop();
		cur_max.pop();
	}

	size_t size() const { return values.size(); }

	void dump() const {
		int n = values.size();
		
		auto container = values._Get_container();
		for (auto it : container) {
			cout << it << " ";
		}
		cout << "| max=" << (size() > 0 ? max() : -1) << endl;
		
	}
};

template <class Type=int>
class max_queue {
	int win_size;
	max_stack<Type> in;
	max_stack<Type> out;
public:
	max_queue(int win_size)
		: win_size(win_size) { }

	size_t size() const { return in.size() + out.size(); }

	void push(const Type& item) {
		
		if (in.size() < win_size) {
			if (!out.empty()) {
				out.pop();
			}
			in.push(item);
		} else {
			while (!in.empty()) {
				out.push(in.top());
				in.pop();
			}
			out.pop();
			in.push(item);
		}
	}

	bool empty() const { return in.empty() && out.empty(); }

	Type max() const {
		if (empty()) throw runtime_error("stack is empty");

		if (in.empty()) return out.max();
		Type max_item = in.max();
		if (out.empty()) return max_item;
		return std::max(max_item, out.max());
	}

	void dump() const {
		cout << "Total size=" << size() << endl;
		cout << "[IN]: ";in.dump();
		cout << endl;

		cout << "[OUT]: ";out.dump();
		cout << endl;

		cout << "-----------" << endl;
	}
};

int main() {
	int n, m, max_idx = 0;
	queue<int> q;

	cin >> n;
	for (int i = 0; i < n; ++i) {
		int value;
		cin >> value;
		q.push(value);
	}
	cin >> m;
	max_queue<int> qm(m);
	while (!q.empty()) {
		qm.push(q.front());
		q.pop();
		//qm.dump();
		if (qm.size() == m) {
			cout << qm.max() << " ";
		}
	}

	return 0;
}