#ifndef __MAX__STACK__HPP__
#define __MAX__STACK__HPP__

#include <stdexcept>

template <class Type = int>
class max_stack {
private:
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
			}
			else if (m_size + 1 >= m_capacity) {
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

		~stack() {
			clear();
		}

		void clear() {
			delete[] data;
			m_capacity = m_size = 0;
		}
    };
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