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
#include <functional>


template <class key_t, class value_t>
class map {
	const int DEFAULT_CAPACITY = 32;
private:
	enum bucket_status {
		EMPTY=0,
		BUSSY,
		DELETED
	};

	struct bucket_t {
		key_t key;
		value_t value;
		bucket_status status;
		bucket_t()
			: status(bucket_status::EMPTY) {}
	};
protected:
	size_t m_size;
	size_t m_capacity;
	bucket_t* m_bucket;
private:

	template <const char*>
	size_t hash(const char* key) const {
		unsigned int b = 378551;
		unsigned int a = 63689;
		unsigned int hash = 0;

		for (std::size_t i = 0; i < key[i] != '\0'; i++) {
			hash = hash * a + key[i];
			a = a * b;
		}

		return (hash & 0x7FFFFFFF) % m_capacity;
	}

	template <long>
	size_t hash(long key) const {
		key ^= (key << 13);
		key ^= (key >> 17);
		key ^= (key << 5);
		return key % m_capacity;
	}

	size_t get_hash(const key_t& key) const {
		std::hash<key_t> h;
		return h(key) % m_capacity;
	}

	void rehash() {
		// TODO
	}

	bool can_insert_at(size_t idx) const { return m_bucket[idx].status == bucket_status::EMPTY || m_bucket[idx].status == bucket_status::DELETED; }
	
	void put_item(size_t idx, const key_t& key, const value_t& value) {
		m_bucket[idx].key = key;
		m_bucket[idx].value = value;
		m_bucket[idx].status = bucket_status::BUSSY;
	}

	void ensure_capacity() {
		const double MAX_LOAD_FACTOR = 0.75;
		double load_factor = m_size / (double)m_capacity;
		if (load_factor >= MAX_LOAD_FACTOR) {
			bucket_t* m_old_bucket = m_bucket;
			size_t m_old_capacity = m_capacity;
			
			// Resize
			m_capacity *= 2;
			m_bucket = new bucket_t[m_capacity];

			// Rehash			
			for (size_t i = 0; i < m_old_capacity; ++i) {
				size_t idx = get_hash(m_old_bucket[i].key);

				if (can_insert_at(idx)) {
					put_item(idx, m_old_bucket[i].key, m_old_bucket[i].value);
				} else { // Linear probe
					bool saved = false;
					size_t n = 0;
					while (n < m_capacity && !saved) {
						idx = (idx + 1) % m_capacity;
						if (can_insert_at(idx)) {
							put_item(idx, m_old_bucket[i].key, m_old_bucket[i].value);
							saved = true;
						}
						++n;
					}
					if (saved == false) throw std::runtime_error("[ensure_capacity] rehash procedure failed");
				}
			}

			delete[] m_old_bucket;
			
		}
	}
public:

	map()
		: m_size(0), m_capacity(DEFAULT_CAPACITY), m_bucket(nullptr) {
		m_bucket = new bucket_t[m_capacity];
	}

	map(const map& other) {
		// TODO
	}

	~map() { clear(); }

	map& operator=(const map& rhs) {
		if (this != &rhs) {
			// TODO
		}
		return *this;
	}

	const value_t& operator[](const key_t& key) const {
		/*ensure_capacity();
		size_t idx = get_hash(key);

		if (can_insert_at(idx)) {
			++m_size;
			put_item(idx, key, value_t());
		}

		return m_bucket[idx].value;*/
	}

	value_t& operator[](const key_t& key) {
		ensure_capacity();
		size_t idx = get_hash(key);

		if (can_insert_at(idx)) {
			put_item(idx, key, value_t());
			++m_size;
		}

		return m_bucket[idx].value;
	}

	void insert(const key_t& key, const value_t& value) {
		ensure_capacity();
		size_t idx = get_hash(key);
		if (can_insert_at(idx)) {
			put_item(idx, key, value);
			++m_size;
		}
	}

	void remove(const key_t& key) {
		// TODO
	}

	value_t find(const key_t& key) const {
		// TODO
	}

	bool contains(const key_t& key) const {
		// TODO
		return false;
	}

	void dump() const {

	}

	void clear() {
		delete[] m_bucket;
		m_size = m_capacity = 0;
		m_bucket = nullptr;
	}

	bool empty() const { return m_size == 0; }

	size_t size() const { return m_size; }
};

#endif // !__MAP__HPP__