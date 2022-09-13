#include <iostream>
#include <vector>
#include <functional>
#include <cmath>
using namespace std;

template <class Type = int>
class splay_tree_t {
private:
    splay_tree_t(const splay_tree_t&) = delete;
    splay_tree_t& operator=(const splay_tree_t&) = delete;
private:
    struct node_t {
        Type key;
        struct node_t* parent;
        struct node_t* left;
        struct node_t* right;

        node_t(const Type& key)
            : key(key), parent(nullptr), left(nullptr), right(nullptr) { }
    };
protected:
    node_t* root;
public:
    splay_tree_t()
        : root(nullptr) {
        root = build_tree_helper();
    }

    ~splay_tree_t() { clear(); }

private:
    void destroy_tree_helper(node_t* node) {
        if (node == nullptr) return;
        destroy_tree_helper(node->left);
        destroy_tree_helper(node->right);
        delete node;
    }

    void in_order_traverse_helper(node_t* node, function<void(const Type& key)> f) {
        if (node == nullptr) return;
        in_order_traverse_helper(node->left, f);
        f(node->key);
        in_order_traverse_helper(node->right, f);
    }

    void pre_order_traverse_helper(node_t* node, function<void(const Type& key)> f) {
        if (node == nullptr) return;
        f(node->key);
        pre_order_traverse_helper(node->left, f);
        pre_order_traverse_helper(node->right, f);
    }

    void helper_post_order_traverse(node_t* node, function<void(const Type& key)> f) {
        if (node == nullptr) return;
        helper_post_order_traverse(node->left, f);
        helper_post_order_traverse(node->right, f);
        f(node->key);
    }

    node_t* build_tree_helper() {
        int n;
        cin >> n;

        if (n == 0) return nullptr;

        vector<node_t*> A(n, nullptr);
        vector<pair<int, int>> children(n);

        for (int i = 0; i < n; ++i) {
            int key, left, right;
            cin >> key >> left >> right;
            A[i] = new node_t(key);
            children[i] = make_pair(left, right);
        }

        for (int i = 0; i < n; ++i) {
            if (children[i].first != -1) {
                A[i]->left = A[children[i].first];
                A[children[i].first]->parent = A[i];
            }
            if (children[i].second != -1) {
                A[i]->right = A[children[i].second];
                A[children[i].second]->parent = A[i];
            }
        }
        return A[0];
    }

    void split(node_t* u) {
        // TODO
    }

    void splay(node_t* u) {
        // TODO
    }

    void merge(node_t* v1, node_t* v2) {
        // TODO
    }

    node_t* insert_helper(node_t* node, const Type& key) {
        // TODO
        if (node == nullptr) {
            node = new node_t(key);
            //splay(node);
        }
        if (node->key < key) {
            node->left = insert_helper(node->left, key);
        } else if (node->key > key) {
            node->left = insert_helper(node->left, key);
        } // else key is already in the tree

        return node;
    }
public:
    void in_order_traverse(function<void(const Type& key)> f) { in_order_traverse_helper(root, f); }
    void pre_order_traverse(function<void(const Type& key)> f) { pre_order_traverse_helper(root, f); }
    void post_order_traverse(function<void(const Type& key)> f) { helper_post_order_traverse(root, f); }

    void clear() { destroy_tree_helper(root); }
    bool empty() const { return root == nullptr; }

    void insert(const Type& key) {
        // TODO
        root = insert_helper(root, key);
    }

    bool search(const Type& key) {
        // TODO
    }

    void remove(const Type& key) {
        // TODO
        //remove_helper(root, key);
    }
};

int main() {
    splay_tree_t<int> tree;
    return 0;
}