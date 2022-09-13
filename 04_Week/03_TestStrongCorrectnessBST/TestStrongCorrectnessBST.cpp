#include <iostream>
#include <vector>
#include <functional>
#include <cmath>
using namespace std;

template <class Type = int>
class binary_search_tree_t {
private:
    binary_search_tree_t(const binary_search_tree_t&) = delete;
    binary_search_tree_t& operator=(const binary_search_tree_t&) = delete;
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
    binary_search_tree_t()
        : root(nullptr) {
        root = helper_build_tree();
    }

    ~binary_search_tree_t() { clear(); }

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

    bool is_level_correct(node_t* parent, node_t* left, node_t* right, node_t* grand_parent=nullptr) const {
        return (parent == nullptr) || ((left == nullptr || left->key < parent->key) && (right == nullptr || parent->key < right->key));
    }

    bool helper_is_correct_bst(node_t* node, double min_value, double max_value) {
        if (node == nullptr) return true;
        Type key = node->key;
        node_t* left = node->left, * right = node->right;

        //cout << "key=" << key << ", min=" << min_value << ", max=" << max_value << endl;
        
        bool current = is_level_correct(node, left, right);
        if (current == false) return false;
        node_t* parent = node->parent;
        if (parent != nullptr) {
            // Left subtree
            if (parent->left == node && key < min_value) return false;
            // Right subtree
            if (parent->right == node && key > max_value) return false;
        }

        bool L = helper_is_correct_bst(node->left, min_value, key);
        bool R = helper_is_correct_bst(node->right, key, max_value);

        return L && R;
    }
public:
    void in_order_traverse(function<void(const Type& key)> f) { helper_in_order_traverse(root, f); }
    void pre_order_traverse(function<void(const Type& key)> f) { helper_pre_order_traverse(root, f); }
    void post_order_traverse(function<void(const Type& key)> f) { helper_post_order_traverse(root, f); }

    void clear() { helper_destroy_tree(root); }
    bool empty() const { return root == nullptr; }

    bool is_correct_bst() {
        
        return root == nullptr || helper_is_correct_bst(root, -INFINITY, +INFINITY);
    }
};

int main() {
    binary_search_tree_t<int> tree;
    cout << (tree.is_correct_bst() ? "CORRECT" : "INCORRECT") << endl;
    return 0;
}