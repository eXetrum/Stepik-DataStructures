#ifndef __MAP__HPP__
#define __MAP__HPP__

#include <stdexcept>

template <class key_t, class value_t>
class map {
protected:
	int m_size;
	int m_capacity;
	key_t* ket_set;
	value_t* value_set;
private:
	void rehash() {
		// TODO
	}
public:
	map() {
		// TODO
	}

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

	const value_t& operator[](int index) const {
		// TODO
	}

	value_t& operator[](int index) {
		// TODO
	}

	bool empty() const { return m_size == 0; }

	int size() const { return m_size; }

	void erase(const key_t& key) {
		// TODO
	}

};

#endif // !__MAP__HPP__