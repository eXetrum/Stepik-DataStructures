#ifndef __STACK__HPP__
#define __STACK__HPP__

#include <iostream>
#include <stdexcept>

template <class T>
class stack {
protected:
	T* data;
	size_t m_capacity;
	size_t m_size;
private:
	void ensure_capacity() {
		// TODO
	}
public:
	stack()
		: data(nullptr), m_capacity(0), m_size(0) { }

	stack(const stack& s) { 
		// TODO
	}

	stack& operator=(const stack& rhs) {
		// TODO
		return *this;
	}

	bool empty() const { return m_size == 0; }

	size_t size() const { return m_size; }

	T top() const {
		if(empty()) throw std::runtime
		return data[0];
	}

	~stack() { 
		clear();
	}

	void clear() {
		delete[] data;
		m_capacity = m_size = 0;
	}
};

#endif // !__STACK__HPP__