#ifndef __PRIORITY__QUEUE__H__
#define __PRIORITY__QUEUE__H__

#include <iterator>
#include <stdexcept>

template <class Type = int, class comparator = std::less<Type>>
class priority_queue {
    const int DEFAULT_CAPACITY = 32;
protected:
    int m_size;
    size_t m_capacity;
    Type* m_data;
    comparator cmp;
private:
    PriorityQueue(const PriorityQueue&) = delete;
    PriorityQueue& operator=(const PriorityQueue&) = delete;
public:
    PriorityQueue()
        : m_size(0), m_capacity(DEFAULT_CAPACITY) {

        m_data = new Type[m_capacity];
    }

    ~PriorityQueue() { delete[] m_data; }

    // Public interface
    bool is_empty() const { return m_size == 0; }
    size_t size() const { return m_size; }

    void push(const Type& data) {
        ensure_capacity();
        m_data[m_size] = data;
        ++m_size;
        heapify_up(m_size - 1);
    }

    Type pool() {
        if (is_empty()) throw std::out_of_range("queue is empty");

        Type top = m_data[0];
        m_data[0] = m_data[m_size - 1];
        --m_size;

        heapify_down(0);

        return top;
    }

    Type peek() const {
        if (is_empty()) throw std::out_of_range("queue is empty");
        return m_data[0];
    }

    //data_type remove(iterator it);
    //void change_priority(iterator it, Type p);

    void dump() const {
        std::cout << "Queue: " << std::endl;
        for (int i = 0; i < m_size; ++i) {
            std::cout << m_data[i] << " ";
        }
        std::cout << std::endl << std::endl;
    }
private:
    void ensure_capacity() {
        const double factor = 1.616;

        if (m_size == m_capacity) {
            m_capacity = (int)(factor * m_capacity);
            Type* m_old_data = m_data;
            m_data = new Type[m_capacity];
            memcpy(m_data, m_old_data, sizeof(Type) * m_size);
            delete[] m_old_data;
        }
    }

    int parent(int i) const { return (i + 1) / 2 - 1; }
    int left(int i) const { return 2 * i + 1; }
    int right(int i) const { return 2 * i + 2; }

    void heapify_up(int index) {
        int parent_idx;
        while ((parent_idx = parent(index)) >= 0 && cmp(m_data[index], m_data[parent_idx])) {
            std::swap(m_data[index], m_data[parent_idx]);
            index = parent_idx;
        }
    }

    void heapify_down(int index) {
        int L_child, R_child;

        while ((L_child = left(index)) < m_size) {
            int lowest_idx = L_child;
            if ((R_child = right(index)) < m_size && cmp(m_data[R_child], m_data[L_child])) {
                lowest_idx = R_child;
            }

            if (!cmp(m_data[lowest_idx], m_data[index])) break;

            std::swap(m_data[index], m_data[lowest_idx]);

            index = lowest_idx;
        }

    }

};

#endif // !__PRIORITY__QUEUE__H__