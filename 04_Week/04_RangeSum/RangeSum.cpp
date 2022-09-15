#include <iostream>
#include <string>
#include <vector>
#include <functional>
using namespace std;

//#include <vld\vld.h>


#ifdef NDEBUG
#define assert(condition, message) 0
#else
#define assert(condition, message)\
   (!(condition)) ?\
      (std::cerr << "Assertion failed: (" << #condition << "), "\
      << "function " << __FUNCTION__\
      << ", file " << __FILE__\
      << ", line " << __LINE__ << "."\
      << std::endl << message << std::endl, abort(), 0) : 1
#endif

template <class Type = long long>
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
        Type sum;

        node_t(const Type& key)
            : key(key), parent(nullptr), left(nullptr), right(nullptr), sum(key) { }
    };    
protected:
    node_t* root;
public:
    splay_tree_t()
        : root(nullptr) { }
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

    inline void small_rotation(node_t* v) {
        node_t* parent = v->parent;
        if (parent == nullptr) { return; }

        node_t* grandparent = v->parent->parent;
        if (parent->left == v) {
            node_t* m = v->right;
            v->right = parent;
            parent->left = m;
        } else {
            node_t* m = v->left;
            v->left = parent;
            parent->right = m;
        }
        update(parent);
        update(v);
        v->parent = grandparent;
        if (grandparent != nullptr) {
            if (grandparent->left == parent) {
                grandparent->left = v;
            }
            else {
                grandparent->right = v;
            }
        }
    }

    inline void big_rotation(node_t* v) {
        if ((v->parent->left == v && v->parent->parent->left == v->parent)
            || (v->parent->right == v && v->parent->parent->right == v->parent)) {
            // Zig-zig
            small_rotation(v->parent);
            small_rotation(v);
        } else {
            // Zig-zag
            small_rotation(v);
            small_rotation(v);
        }
    }

    inline void update(node_t* v) {
        if (v == nullptr) return;
        v->sum = v->key 
            + (v->left != nullptr ? v->left->sum : 0) 
            + (v->right != nullptr ? v->right->sum : 0);
        if (v->left != nullptr) { v->left->parent = v; }
        if (v->right != nullptr) { v->right->parent = v; }
    }


    inline node_t* splay(node_t* u) {
        if (u == nullptr) return nullptr;
        while (u->parent != nullptr) {
            // No grand parent (we r close to root), do single rotation and stop
            if (u->parent->parent == nullptr) {
                small_rotation(u);
                break;
            }
            // Keep pushing 'u' to the root
            big_rotation(u);
        }
        return u;
    }

    inline node_t* merge(node_t* left, node_t* right) {
        
        // No left subtree -> new root is right subtree
        if (left == nullptr) return right;
        
        // No right subtree -> new root is left subtree
        if (right == nullptr) return left;
        
        // Min in right subtree
        while (right->left != nullptr) { right = right->left; }
        
        // Move to the root and make assign left subtree as left child
        right = splay(right);
        right->left = left;

        // Update sum values 
        update(right);

        return right;
    }

    inline pair<node_t*, node_t*> split(node_t* root, const Type& key) {
        pair<node_t*, node_t*> result;

        pair<node_t*, node_t*> find_and_root = find(root, key);
        root = find_and_root.second;
        result.second = find_and_root.first;
        if (result.second == nullptr) {
            result.first = root;
            return result;
        }
        result.second = splay(result.second);
        result.first = result.second->left;
        result.second->left = nullptr;
        
        // No more parent for left subtree
        if (result.first != nullptr) {
            result.first->parent = nullptr;
        }

        // Update sum value for left/right subtree
        update(result.first);
        update(result.second);

        return result;
    }

    inline pair<node_t*, node_t*> find(node_t* root, const Type& key) {
        node_t* v = root;
        node_t* last = root;
        node_t* next = nullptr;
        while (v != nullptr) {
            if (v->key >= key 
                && (next == nullptr || v->key < next->key)) {
                next = v;
            }
            last = v;
            if (v->key == key) { break; }
            if (v->key < key) { v = v->right; }
            else { v = v->left; }
        }
        root = splay(last);
        return make_pair(next, root);
    }
public:
    void in_order_traverse(function<void(const Type& key)> func) { in_order_traverse_helper(root, func); }
    void pre_order_traverse(function<void(const Type& key)> func) { pre_order_traverse_helper(root, func); }
    void post_order_traverse(function<void(const Type& key)> func) { helper_post_order_traverse(root, func); }

    void clear() { destroy_tree_helper(root); }
    bool empty() const { return root == nullptr; }

    void insert(const Type& x) {
        node_t* left = nullptr;
        node_t* right = nullptr;
        node_t* new_vertex = nullptr;
        pair<node_t*, node_t*> leftRight = split(root, x);
        left = leftRight.first;
        right = leftRight.second;
        if (right == nullptr || right->key != x) { new_vertex = new node_t(x); }
        root = merge(merge(left, new_vertex), right);
    }

    void remove(const Type& x) {
        // Use split and merge technique
        pair<node_t*, node_t*> leftMiddle = split(root, x);
        node_t* left = leftMiddle.first;
        node_t* middle = leftMiddle.second;
        pair<node_t*, node_t*> middleRight = split(middle, x + 1);
        middle = middleRight.first;
        node_t* right = middleRight.second;
        if (middle == nullptr || middle->key != x) {
            root = merge(merge(left, middle), right);
        } else {
            delete middle;
            root = merge(left, right);
        }
    }

    bool find(const Type& x) {
        node_t* left = nullptr;
        node_t* right = nullptr;
        pair<node_t*, node_t*> leftRight = split(root, x);
        left = leftRight.first;
        right = leftRight.second;
        root = merge(left, right);

        return right != nullptr && right->key == x;
    }

    Type range_sum(const Type& L, const Type& R) {
        pair<node_t*, node_t*> leftMiddle = split(root, L);
        node_t* left = leftMiddle.first;
        node_t* middle = leftMiddle.second;
        pair<node_t*, node_t*> middleRight = split(middle, R + 1);
        middle = middleRight.first;
        node_t* right = middleRight.second;
        Type ans = 0;
        // Complete the implementation of sum
        if (middle != nullptr)
            ans = middle->sum;
        middle = merge(middle, right);
        root = merge(left, middle);

        return ans;
    }
};

int main() {

    splay_tree_t<long long> tree;
    function<long long(long long, long long)> f = [](long long x, long long sum) { 
        const long long P = 1000000001;
        return (x + sum) % P;
    };

    long long n, value, L, R, sum = 0;
    string cmd;
    cin >> n;

    for (int i = 0; i < n; ++i) {
        cin >> cmd;
        if (cmd == "+") {
            cin >> value;
            tree.insert(f(value, sum));
        }
        else if (cmd == "-") {
            cin >> value;
            tree.remove(f(value, sum));
        }
        else if (cmd == "?") {
            cin >> value;
            cout << (tree.find(f(value, sum)) ? "Found" : "Not found") << endl;
        }
        else if (cmd == "s") {
            cin >> L >> R;
            sum = tree.range_sum(f(L, sum), f(R, sum));
            cout << sum << endl;
        }
    }

    return 0;
}
