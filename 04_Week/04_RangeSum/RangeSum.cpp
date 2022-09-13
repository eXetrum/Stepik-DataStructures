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
        root = helper_build_tree();
    }

    ~splay_tree_t() { clear(); }

private:
    void helper_destroy_tree(node_t* node) {
        if (node == nullptr) return;
        helper_destroy_tree(node->left);
        helper_destroy_tree(node->right);
        delete node;
    }

    void helper_in_order_traverse(node_t* node, function<void(const Type& key)> f) {
        if (node == nullptr) return;
        helper_in_order_traverse(node->left, f);
        f(node->key);
        helper_in_order_traverse(node->right, f);
    }

    void helper_pre_order_traverse(node_t* node, function<void(const Type& key)> f) {
        if (node == nullptr) return;
        f(node->key);
        helper_pre_order_traverse(node->left, f);
        helper_pre_order_traverse(node->right, f);
    }

    void helper_post_order_traverse(node_t* node, function<void(const Type& key)> f) {
        if (node == nullptr) return;
        helper_post_order_traverse(node->left, f);
        helper_post_order_traverse(node->right, f);
        f(node->key);
    }

    node_t* helper_build_tree() {
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
public:
    void in_order_traverse(function<void(const Type& key)> f) { helper_in_order_traverse(root, f); }
    void pre_order_traverse(function<void(const Type& key)> f) { helper_pre_order_traverse(root, f); }
    void post_order_traverse(function<void(const Type& key)> f) { helper_post_order_traverse(root, f); }

    void clear() { helper_destroy_tree(root); }
    bool empty() const { return root == nullptr; }

    void insert(const Type& key) {
        // TODO
    }

    bool search(const Type& key) {
        // TODO
    }
};

int main() {
    splay_tree_t<int> tree;
    cout << (tree.is_correct_bst() ? "CORRECT" : "INCORRECT") << endl;
    return 0;
}