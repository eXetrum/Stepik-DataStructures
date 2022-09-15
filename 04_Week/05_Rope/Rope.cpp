#include <iostream>
#include <string>
#include <functional>
using namespace std;



class Rope {
    struct node_t {
        string key;
        int w;
        node_t* left;
        node_t* right;

        node_t(const string& s)
            : s(s), w(s.size()), left(nullptr), right(nullptr) { }
    };
private:
    Rope(const Rope&) = delete;
    Rope& operator=(const Rope&) = delete;
protected:
    node_t* root;
private:
    void clear_helper(node_t* node) {
        if (node == nullptr) return;
        clear_helper(node->left);
        clear_helper(node->right);
        delete node;
    }

    void in_order_traverse_helper(node_t* node, function<void(const string& key)> f) {
        if (node == nullptr) return;
        in_order_traverse_helper(node->left, f);
        f(node->key);
        in_order_traverse_helper(node->right, f);
    }

    void pre_order_traverse_helper(node_t* node, function<void(const string& key)> f) {
        if (node == nullptr) return;
        f(node->key);
        pre_order_traverse_helper(node->left, f);
        pre_order_traverse_helper(node->right, f);
    }

    void helper_post_order_traverse(node_t* node, function<void(const string& key)> f) {
        if (node == nullptr) return;
        helper_post_order_traverse(node->left, f);
        helper_post_order_traverse(node->right, f);
        f(node->key);
    }

    node_t* merge(node_t* n1, node_t* n2) {
        return Node(n1, n2, n1.w + n2.w)
    }

public:
    Rope(const string& s) {
        root = new node_t(s);
    }
    ~Rope() { clear(); }
    
    bool empty() const { return root == nullptr; }
    void clear() { clear_helper(root); }
    void in_order_traverse(function<void(const string& key)> func) { in_order_traverse_helper(root, func); }
    void pre_order_traverse(function<void(const string& key)> func) { pre_order_traverse_helper(root, func); }
    void post_order_traverse(function<void(const string& key)> func) { helper_post_order_traverse(root, func); }
};

int main() {
    string S;
    int q, i, j, k;

    cin >> S >> q;
    Rope rope(S);



    while(q-- > 0) {
        cin >> i >> j >> k;
    }

    return 0;
}