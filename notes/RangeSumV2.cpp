#include <tuple>
#include <string>
#include <utility>
#include <cstdint>
#include <cassert>
#include <fstream>
#include <iostream>

namespace BTS
{
    template <class NodeTree>
    NodeTree* search(NodeTree* node, typename NodeTree::value_type key)
    {
        NodeTree* pnode = node;
        while (node)
        {
            if (node->key == key)
            {
                return node;
            }

            pnode = node;
            node = key < node->key ? node->lnode : node->rnode;
        }
        return pnode;
    }

    template <class NodeTree>
    NodeTree* lower_bound(NodeTree* node, typename NodeTree::value_type key)
    {
        NodeTree* out = nullptr;
        while (node)
        {
            if (node->key == key)
            {
                return node;
            }

            if (!(node->key < key))
            {
                out = node;
            }
            node = key < node->key ? node->lnode : node->rnode;
        }
        return out;
    }

    template <class NodeTree>
    NodeTree* upper_bound(NodeTree* node, typename NodeTree::value_type key)
    {
        NodeTree* out = nullptr;
        while (node)
        {
            if ((key < node->key))
            {
                out = node;
            }
            node = key < node->key ? node->lnode : node->rnode;
        }
        return out;
    }

    template <class NodeTree>
    NodeTree* insert(NodeTree* node, typename NodeTree::value_type key)
    {
        NodeTree* pnode = nullptr;
        while (node)
        {
            if (node->key == key)
            {
                return node;
            }

            pnode = node;
            node = key < node->key ? node->lnode : node->rnode;
        }

        auto leaf = new NodeTree{ key };
        if (pnode)
        {
            key < pnode->key ? pnode->lnode = leaf : pnode->rnode = leaf;
            leaf->pnode = pnode;
        }
        return leaf;
    }

    template <class NodeTree>
    NodeTree* max_element(NodeTree* node)
    {
        if (!node) { return node; }

        while (node->rnode)
        {
            node = node->rnode;
        }
        return node;
    }
}

namespace SPLAY
{
    template <class T, class U>
    struct node_type
    {
        using value_type = T;
        using accum_type = U;

        explicit node_type(value_type value) : key(value), sum(value) { }

        value_type key;
        accum_type sum;
        node_type<value_type, accum_type>* pnode = nullptr;
        node_type<value_type, accum_type>* lnode = nullptr;
        node_type<value_type, accum_type>* rnode = nullptr;
    };

    template <class T>
    class set
    {
    public:
        using value_type = T;
        using accum_type = std::int64_t;
        using node_tree = node_type<value_type, accum_type>;

        void insert(value_type key);
        void remove(value_type key);
        bool search(value_type key);
        accum_type sum(value_type lo, value_type hi);

    private:
        void lnode_set(node_tree* parent, node_tree* child);
        void rnode_set(node_tree* parent, node_tree* child);
        void update_sum(node_tree* node);
        void rotate(node_tree* node);
        void splay(node_tree* node);
        node_tree* search(node_tree* root, value_type key);
        node_tree* insert(node_tree* root, value_type key);
        node_tree* remove(node_tree* root, value_type key);
        node_tree* merge(node_tree* x, node_tree* y);
        std::pair<node_tree*, node_tree*> split(node_tree* node);

        node_tree* root = nullptr;
    };

    template <class T>
    void set<T>::insert(value_type key)
    {
        root = insert(root, key);
    }

    template <class T>
    void set<T>::remove(value_type key)
    {
        root = remove(root, key);
    }

    template <class T>
    bool set<T>::search(value_type key)
    {
        root = search(root, key);
        return root && root->key == key;
    }

    template <class T>
    typename set<T>::accum_type set<T>::sum(value_type lo, value_type hi)
    {
        accum_type out = 0;

        node_tree* ltree = nullptr;
        node_tree* mtree = nullptr;
        node_tree* rtree = nullptr;

        auto lb = BTS::lower_bound(root, lo);
        if (lb == nullptr)
        {
            return out;
        }

        std::tie(ltree, mtree) = split(lb);

        auto ub = BTS::upper_bound(mtree, hi);
        if (ub != nullptr)
        {
            std::tie(mtree, rtree) = split(ub);
        }

        if (mtree)
        {
            out = mtree->sum;
        }

        root = merge(ltree, mtree);
        root = merge(root, rtree);

        return out;
    }

    template <class T>
    void set<T>::lnode_set(node_tree* parent, node_tree* child)
    {
        assert(parent);

        if (child)
        {
            child->pnode = parent;
        }
        parent->lnode = child;
    }

    template <class T>
    void set<T>::rnode_set(node_tree* parent, node_tree* child)
    {
        assert(parent);

        if (child)
        {
            child->pnode = parent;
        }
        parent->rnode = child;
    }

    template <class T>
    void set<T>::update_sum(node_tree* node)
    {
        node->sum = node->key;

        if (node->lnode)
        {
            node->sum += node->lnode->sum;
        }

        if (node->rnode)
        {
            node->sum += node->rnode->sum;
        }
    }

    template <class T>
    void set<T>::rotate(node_tree* node)
    {
        assert(node);
        assert(node->pnode);

        node_tree* pnode = node->pnode;
        node_tree* gnode = pnode->pnode;
        node->pnode = gnode;
        if (gnode)
        {
            gnode->lnode == pnode ? gnode->lnode = node : gnode->rnode = node;
        }

        if (pnode->lnode == node)
        {
            lnode_set(pnode, node->rnode);
            rnode_set(node, pnode);
        }
        else
        {
            assert(pnode->rnode == node);
            rnode_set(pnode, node->lnode);
            lnode_set(node, pnode);
        }

        update_sum(pnode);
        update_sum(node);
    }

    template <class T>
    void set<T>::splay(node_tree* node)
    {
        assert(node);

        while (node->pnode)
        {
            node_tree* pnode = node->pnode;
            node_tree* gnode = pnode->pnode;

            if (gnode)
            {
                if ((gnode->lnode == pnode && pnode->lnode == node) ||
                    (gnode->rnode == pnode && pnode->rnode == node))
                {
                    rotate(pnode);
                    rotate(node);
                }
                else
                {
                    rotate(node);
                    rotate(node);
                }
            }
            else
            {
                rotate(node);
            }
        }
    }

    template <class T>
    typename set<T>::node_tree* set<T>::search(node_tree* root, value_type key)
    {
        if (!root) { return root; }

        auto node = BTS::search(root, key);
        assert(node);
        splay(node);
        return node;
    }

    template <class T>
    typename set<T>::node_tree* set<T>::insert(node_tree* root, value_type key)
    {
        auto node = BTS::insert(root, key);
        splay(node);
        return node;
    }

    template <class T>
    typename set<T>::node_tree* set<T>::remove(node_tree* root, value_type key)
    {
        if (!root) { return root; }

        auto node = search(root, key);
        assert(node);
        if (node->key != key)
        {
            return node;
        }

        auto ltree = node->lnode;
        auto rtree = node->rnode;
        if (ltree) { ltree->pnode = nullptr; }
        if (rtree) { rtree->pnode = nullptr; }

        delete node;
        return merge(ltree, rtree);
    }

    template <class T>
    std::pair<typename set<T>::node_tree*, typename set<T>::node_tree*> set<T>::split(node_tree* node)
    {
        if (!node) { return { nullptr, nullptr }; }

        splay(node);
        auto ltree = node->lnode;
        auto rtree = node;

        if (node->lnode)
        {
            node->lnode->pnode = nullptr;
            node->lnode = nullptr;
        }
        update_sum(node);

        return { ltree, rtree };
    }

    template <class T>
    typename set<T>::node_tree* set<T>::merge(node_tree* x, node_tree* y)
    {
        if (!x) { return y; }
        if (!y) { return x; }

        auto node = BTS::max_element(x);
        splay(node);
        rnode_set(node, y);
        update_sum(node);
        return node;
    }
}

int64_t f(int64_t x, int64_t s)
{
    constexpr int64_t mod = 1000000001;
    return ((x % mod) + (s % mod)) % mod;
}

#define istream std::cin
#define ostream std::cout

int main(int, char**)
{
    //std::fstream istream("input.txt", istream.in);
    //std::fstream ostream("output.txt", ostream.trunc | ostream.out);

    std::size_t count = 0u;
    istream >> count;

    SPLAY::set<int64_t> st;

    int64_t s = 0;
    for (std::size_t i = 0u; i < count; ++i)
    {
        std::string query;
        istream >> query;

        if (query == "+")
        {
            int64_t num = 0;
            istream >> num;
            st.insert(f(num, s));
        }
        else if (query == "-")
        {
            int64_t num = 0;
            istream >> num;
            st.remove(f(num, s));
        }
        else if (query == "?")
        {
            int64_t num = 0;
            istream >> num;
            ostream << (st.search(f(num, s)) ? "Found" : "Not found") << std::endl;
        }
        else
        {
            int64_t lo = 0, hi = 0;
            istream >> lo >> hi;
            s = st.sum(f(lo, s), f(hi, s));
            ostream << s << std::endl;
        }
    }

    return 0;
}