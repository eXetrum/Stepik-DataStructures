#ifndef __MAX__STACK__HPP__
#define __MAX__STACK__HPP__

#include "stack.hpp"
#include <stdexcept>

template <class Type = int>
class max_stack {
protected:
	stack<Type> values;
	stack<Type> cur_max;
public:
	max_stack() { }

	max_stack(const max_stack& other)
		: values(other.values), cur_max(other.cur_max) { }

	max_stack& operator=(const max_stack& rhs) {
		if (this != &rhs) {
			max_stack temp(rhs);
			swap(values, temp.values);
			swap(cur_max, temp.cur_max);
		}
		return *this;
	}

	bool empty() const { return values.empty(); }

	size_t size() const { return values.size(); }

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

#endif // !__MAX__STACK__HPP__