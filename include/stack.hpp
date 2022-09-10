#ifndef __STACK__HPP__
#define __STACK__HPP__

#include <stdexcept>

template <class Type>
class stack {
protected:
	Type* data;
	size_t m_capacity;
	size_t m_size;
private:
	void swap(Type& item1, Type& item2) {
		Type temp = item1;
		item1 = item2;
		item2 = temp;
	}
	void ensure_capacity() {
		const int MULT_FACTOR = 2;
		if (m_capacity == 0) {
			m_capacity = 1;
			data = new Type[m_capacity];
		} else if(m_size + 1 >= m_capacity) {
			m_capacity *= MULT_FACTOR;
			Type* old_data = data;
			data = new Type[m_capacity];
			for (int i = 0; i < m_size; ++i) { data[i] = old_data[i]; }
		}
	}
public:
	stack()
		: data(nullptr), m_capacity(0), m_size(0) { }

	stack(const stack& other) 
		: data(nullptr), m_capacity(other.m_capacity), m_size(other.m_size) { 
		
		data = new Type[m_capacity];
		for (int i = 0; i < m_size; ++i) data[i] = other.data[i];
	}

	stack& operator=(const stack& rhs) {
		if (this != &rhs) {
			stack temp(rhs);
			swap(data, temp.data);
			swap(m_capacity, temp.m_capacity);
			swap(m_size, temp.m_size);
		}
		return *this;
	}

	~stack() { clear(); }

	bool empty() const { return m_size == 0; }

	size_t size() const { return m_size; }

	void push(const Type& item) {
		ensure_capacity();
		data[m_size++] = item;
	}

	Type top() const {
		if (empty()) throw std::runtime_error("empty stack");
		return data[m_size - 1];
	}

	Type pop() {
		if (empty()) throw std::runtime_error("empty stack");
		Type item = data[m_size - 1];
		--m_size;
		return item;
	}

	void clear() {
		delete[] data;
		m_capacity = m_size = 0;
	}
};

#endif // !__STACK__HPP__