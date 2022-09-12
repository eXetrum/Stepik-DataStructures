#include <iostream>
#include <vector>
using namespace std;

template <class Type=int>
struct node_t {
    Type key;
    struct node_t* left;
    struct node_t* right;

    node_t(const Type& key)
        : key(key), left(nullptr), right(nullptr) { }
};

template <class Type = int>
void destroy_tree(node_t<Type>* node) {
    if (node == nullptr) return;
    destroy_tree(node->left);
    destroy_tree(node->right);
    delete node;
}

template <class Type=int>
void in_order_traverse(node_t<Type>* node) {
    if (node == nullptr) return;
    in_order_traverse(node->left);
    cout << node->key << " ";
    in_order_traverse(node->right);
}

template <class Type = int>
void pre_order_traverse(node_t<Type>* node) {
    if (node == nullptr) return;
    cout << node->key << " ";
    pre_order_traverse(node->left);
    pre_order_traverse(node->right);
}

template <class Type = int>
void post_order_traverse(node_t<Type>* node) {
    if (node == nullptr) return;
    post_order_traverse(node->left);
    post_order_traverse(node->right);
    cout << node->key << " ";
}

int main() {
    int n;
    cin >> n;

    vector<node_t<int>*> A(n, nullptr);
    vector<pair<int, int>> children(n);
    for (int i = 0; i < n; ++i) {
        int key, left, right;
        cin >> key >> left >> right;
        A[i] = new node_t<int>(key);
        children[i] = make_pair(left, right);
    }
    
    node_t<int>* root = A[0];

    for (int i = 0; i < n; ++i) {
        if(children[i].first != -1) 
            A[i]->left = A[children[i].first];
        if(children[i].second != -1)
            A[i]->right = A[children[i].second];
    }

    in_order_traverse(root); cout << endl;
    pre_order_traverse(root); cout << endl;
    post_order_traverse(root); cout << endl;

    destroy_tree(root);
    return 0;
}