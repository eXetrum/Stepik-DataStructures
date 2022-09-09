#ifndef __MAX__QUEUE__H__
#define __MAX__QUEUE__H__

#include <stdexcept>
#include "max_stack.hpp"

template <class Type = int>
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
		cout << "[IN]: "; in.dump();
		cout << endl;

		cout << "[OUT]: "; out.dump();
		cout << endl;

		cout << "-----------" << endl;
	}
};

#endif // !__MAX__QUEUE__H__