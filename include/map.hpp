#ifndef __MAP__HPP__
#define __MAP__HPP__

/*
#include <iterator> // For std::forward_iterator_tag
#include <cstddef>  // For std::ptrdiff_t

struct Iterator {
	using iterator_category = std::forward_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = int;
	using pointer = int*;  // or also value_type*
	using reference = int&;  // or also value_type&
};*/
#include <stdexcept>

template <class key_t, class value_t>
class map {
protected:
	int m_size;
	int m_capacity;
	key_t* key_set;
	value_t* value_set;
private:
	void rehash() {
		// TODO
	}
public:
	map()
		: m_size(0), m_capacity(0), key_set(nullptr), value_set(nullptr) { }

	map(const map& other) {
		// TODO
	}

	~map() {
		// TODO
	}

	map& operator=(const map& rhs) {
		if (this != &rhs) {
			// TODO
		}
		return *this;
	}

	const value_t& operator[](const key_t& key) const {
		// TODO
	}

	value_t& operator[](const key_t& key) {
		// TODO
	}

	void insert(const key_t& key, const value_t& value) {
		// TODO
	}

	void remove(const key_t& key) {
		// TODO
	}

	value_t find(const key_t& key) const {
		// TODO
	}

	bool contains(const key_t& key) const {
		// TODO
	}

	bool empty() const { return m_size == 0; }

	int size() const { return m_size; }
};

#endif // !__MAP__HPP__