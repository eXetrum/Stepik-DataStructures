#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <cmath>
//#include <cassert>
#include <stack>
#include <set>
using namespace std;

#include <vld\vld.h>


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

    set<int> S; // Let S = {} (an empty global set of integers)
public:
    ///////////////////////////////////////////////////////////////////////
    void dump() {
        S.clear();
        print(root, 0);
    }
private:

    void indent(int level) {
        for (int i = 0; i < level; ++i) {				// i in[0..level)
            if (S.find(i) != S.end()) cout << "| ";		// if i \in S print "| "
            else cout << "  ";							// else print "  "
        }
    }

    void tee(int level) {
        indent(level);
        cout << "|-";
        S.insert(level);
    }

    void ell(int level) {
        indent(level);
        cout << "`-";
        S.erase(level);
    }


    void print(node_t* node, int level) {
        if (node == nullptr) {
            cout << "[null]" << endl; //with newline) // only prints when 1 child is missing
            return;
        }

        cout << node->key << endl; //print(node label with newline)
        if (node->left != nullptr || node->right != nullptr) {
            tee(level);
            print(node->left, level + 1);
            ell(level);
            print(node->right, level + 1);
        }
    }
    ///////////////////////////////////////////////////////////////////////

protected:
    node_t* root;
    Type sum;
public:
    splay_tree_t()
        : root(nullptr), sum(0) {
    }

    ~splay_tree_t() { clear(); }

private:
public:
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

    node_t* left_rotation(node_t* x) {
        if (x == nullptr) return nullptr;

        node_t* z = x->parent;
        node_t* y = x->right;
        if (z != nullptr) {
            if (z->left == x) {
                z->left = y;
            }
            else {
                z->right = y;
            }
        }

        node_t* tmp = y->left;
        y->left = x;
        x->right = tmp;

        x->parent = y;
        y->parent = z;
        if (x->right != nullptr)
            x->right->parent = x;

        update_sum(x);
        return y;

        //node_t* z = x->parent;
        //
        //node_t* y = x->right; //
        //x->right = y->left; //
        //y->left = x; //

        //x->parent = y;
        //y->parent = z;
        //return y;
    }

    node_t* right_rotation(node_t* x) {
        if (x == nullptr) return nullptr;

        node_t* z = x->parent;
        node_t* y = x->left;
        if (z != nullptr) {
            if (z->right == x) {
                z->right = y;
            }
            else {
                z->left = y;
            }
        }

        node_t* tmp = y->right;
        y->right = x;
        x->left = tmp;

        x->parent = y;
        y->parent = z;
        if (x->left != nullptr)
            x->left->parent = x;

        update_sum(x);
        return y;


        /*node_t* z = x->parent;
        node_t* y = x->left;
        x->left = y->right;
        y->right = x;
        x->parent = y;
        y->parent = z;
        return y;*/
    }

    void small_rotation(node_t* v) {
        node_t* parent = v->parent;
        if (parent == nullptr) {
            return;
        }

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

    void big_rotation(node_t* v) {
        if (v->parent->left == v && v->parent->parent->left == v->parent) {
            // Zig-zig
            small_rotation(v->parent);
            small_rotation(v);
        } else if (v->parent->right == v && v->parent->parent->right == v->parent) {
            // Zig-zig
            small_rotation(v->parent);
            small_rotation(v);
        }
        else {
            // Zig-zag
            small_rotation(v);
            small_rotation(v);
        }
    }

    void update(node_t* v) {
        if (v == nullptr) return;
        v->sum = v->key 
            + (v->left != nullptr ? v->left->sum : 0) 
            + (v->right != nullptr ? v->right->sum : 0);
        if (v->left != nullptr) {
            v->left->parent = v;
        }
        if (v->right != nullptr) {
            v->right->parent = v;
        }
    }


    node_t* splay(node_t* u) {
        if (u == nullptr) return nullptr;
        while (u->parent != nullptr) {
            if (u->parent->parent == nullptr) {
                small_rotation(u);
                break;
            }
            big_rotation(u);
        }
        return u;
        
        //while (u != nullptr && u->parent != nullptr) {
        //    // Single Zig(right) or Zag(left) rotation
        //    if (u->parent == root || u->parent->parent == nullptr) {
        //        // Zig
        //        if (u == u->parent->left) { /*cout << "ZIG" << endl; */ u = right_rotation(u->parent); }
        //        // Zag
        //        else { /*cout << "ZAG" << endl; */ u = left_rotation(u->parent); }
        //    }
        //    else {
        //        node_t* p = u->parent;
        //        node_t* g = p->parent;
        //        //if(g == nullptr) break;
        //        // Zig-Zig
        //        if (u == p->left && p == g->left) {
        //            //cout << "ZIG-ZIG" << endl;
        //            u = right_rotation(g);
        //            u = right_rotation(p);
        //            // Zag-Zag
        //        }
        //        else if (u == p->right && p == g->right) {
        //            //cout << "ZAG-ZAG" << endl;
        //            u = left_rotation(g);
        //            u = left_rotation(p);
        //            // Zig-Zag
        //        }
        //        else if (u == p->left && p == g->right) {
        //            //cout << "ZIG-ZAG" << endl;
        //            u = right_rotation(p);
        //            u = left_rotation(g);
        //            // Zag-Zig
        //        }
        //        else {
        //            //cout << "ZAG-ZIG" << endl;
        //            u = left_rotation(p);
        //            u = right_rotation(g);
        //        }
        //    }

        //}
        //root = u;
    }

    node_t* get_max_node(node_t* v) {
        if (v == nullptr) return nullptr;

        node_t* max_node = v;
        node_t* left_max = get_max_node(v->left);
        node_t* right_max = get_max_node(v->right);

        if (left_max != nullptr && left_max->key > max_node->key) {
            max_node = left_max;
        }

        if (right_max != nullptr && right_max->key > max_node->key) {
            max_node = right_max;
        }

        return max_node;
    }

    node_t* merge(node_t* left, node_t* right) {
        if (left == nullptr) return right;
        if (right == nullptr) return left;
        while (right->left != nullptr) {
            right = right->left;
        }
        right = splay(right);
        right->left = left;
        update(right);
        return right;
    }


    pair<node_t*, node_t*> split(node_t* root, const Type& key) {
        pair<node_t*, node_t*> result;

        pair<node_t*, node_t*> findAndRoot = find(root, key);
        root = findAndRoot.second;
        result.second = findAndRoot.first;
        if (result.second == nullptr) {
            result.first = root;
            return result;
        }
        result.second = splay(result.second);
        result.first = result.second->left;
        result.second->left = nullptr;
        if (result.first != nullptr) {
            result.first->parent = nullptr;
        }
        update(result.first);
        update(result.second);
        return result;
    }


    void insert(const Type& x) {
        // System.out.println("Inserting "+x);
        node_t* left = nullptr;
        node_t* right = nullptr;
        node_t* new_vertex = nullptr;
        pair<node_t*, node_t*> leftRight = split(root, x);
        left = leftRight.first;
        right = leftRight.second;
        if (right == nullptr || right->key != x) {
            new_vertex = new node_t(x);// , x, null, null, null);
        }
        root = merge(merge(left, new_vertex), right);
    }

    void remove(const Type& x) {
        // use split and merge
        pair<node_t*, node_t*> leftMiddle = split(root, x);
        node_t* left = leftMiddle.first;
        node_t* middle = leftMiddle.second;
        pair<node_t*, node_t*> middleRight = split(middle, x + 1);
        middle = middleRight.first;
        node_t* right = middleRight.second;
        if (middle == nullptr || middle->key != x) {
            root = merge(merge(left, middle), right);
        } else {
            delete middle; //middle = nullptr;
            root = merge(left, right);
        }
    }

    pair<node_t*, node_t*> find(node_t* root, const Type& key) {
        node_t* v = root;
        node_t* last = root;
        node_t* next = nullptr;
        while (v != nullptr) {
            if (v->key >= key && (next == nullptr || v->key < next->key)) {
                next = v;
            }
            last = v;
            if (v->key == key) {
                break;
            }
            if (v->key < key) {
                v = v->right;
            } else {
                v = v->left;
            }
        }
        root = splay(last);

        return make_pair(next, root);
    }

    bool find(const Type& x) {
        // By looking into find(Vertex, int) if found,
        // the right of the returned VertexPair is the node
        // the left is the smallest bigger node
        /* Does this work? */
        // 	VertexPair leftRight = find(root, x);
        // 	if(leftRight.right != null && leftRight.right.key == x) {
        // 		return true;
        // 	}
        //     return false;
        node_t* left = nullptr;
        node_t* right = nullptr;
        pair<node_t*, node_t*> leftRight = split(root, x);
        left = leftRight.first;
        right = leftRight.second;
        if (right == nullptr || right->key != x) {
            root = merge(left, right);
            return false;
        } else {
            root = merge(left, right);
            return true;
        }
    }


    //Type sum_childs(node_t* v1, node_t* v2) const {
    //    Type sum = 0;
    //    if (v1 != nullptr)
    //        sum = sum + v1->sum;

    //    if (v2 != nullptr)
    //        sum = sum + v2->sum;

    //    return sum;
    //}

    //void update_sum(node_t* y) {
    //    while (y != nullptr) {
    //        y->sum = y->key + sum_childs(y->left, y->right);
    //        y = y->parent;
    //    }
    //}

    //void insert_helper(const Type& key) {
    //    if (root == nullptr) { root = new node_t(key); return; }

    //    // Keep parent
    //    node_t* y = nullptr, * cursor = root;
    //    while (cursor != nullptr) {
    //        y = cursor;
    //        if (key < cursor->key) { cursor = cursor->left; }
    //        else { cursor = cursor->right; }
    //    }

    //    // No duplicates, but move current node to root
    //    if (y->key == key) { splay(y); return; }

    //    // Insert as left/right child
    //    node_t* node = new node_t(key);
    //    node->parent = y;

    //    update_sum(y);

    //    if (key < y->key) { y->left = node; }
    //    else { y->right = node; }

    //    splay(node);
    //}

    //node_t* find_helper(const Type& key) {
    //    if (root == nullptr) return nullptr;

    //    // Find predecessor
    //    node_t* y = nullptr, * cursor = root;
    //    while (cursor != nullptr) {
    //        y = cursor;
    //        if (key == cursor->key) {
    //            splay(cursor);
    //            return cursor;
    //        }
    //        if (key < cursor->key) { cursor = cursor->left; }
    //        else { cursor = cursor->right; }
    //    }
    //    splay(y);
    //    return y;
    //}

    //void remove_helper(const Type& key) {
    //    if (empty()) return;
    //    node_t* x = find_helper(key);
    //    if (x == nullptr || x->key != key) return;
    //    node_t* v1 = x->left, * v2 = x->right;
    //    delete x;
    //    merge(v1, v2);
    //}

    //Type sum_helper(const Type& L, const Type& R) {
    //    if (empty() || L > R) {
    //        sum = 0;
    //        return sum;
    //    }

    //    Type ssum = root->sum;

    //    node_t* T1 = find_helper(L);

    //    //dump2();

    //    if (T1->left != nullptr)
    //        ssum -= T1->left->sum;
    //    if (T1->key < L) ssum -= T1->key;

    //    node_t* T2 = find_helper(R);

    //    //dump2();

    //    if (T2->right != nullptr)
    //        ssum -= T2->right->sum;
    //    if (T2->key > R) ssum -= T2->key;

    //    Type S = 0;
    //    function<void(const Type& k)> fu = [&L, &R, &S](const Type& key) {
    //        cout << "-> key=" << key << ", sum=" << S;
    //        if (key >= L && key <= R)
    //            S += key;
    //    };
    //    in_order_traverse_helper(root, fu);
    //    cout << endl;

    //    assert(ssum == S, "test1 != test2, ssum=" << ssum << ", S=" << S << endl);
    //    sum = ssum;
    //    // 
    //    //sum = sum_helper_iter(root, L, R);

    //    
    //    //S = sum_helper_recursive(T2, S, L, R);
    //    //sum = S;
    //    return sum;
    //}
public:
    void in_order_traverse(function<void(const Type& key)> func) { in_order_traverse_helper(root, func); }
    void pre_order_traverse(function<void(const Type& key)> func) { pre_order_traverse_helper(root, func); }
    void post_order_traverse(function<void(const Type& key)> func) { helper_post_order_traverse(root, func); }

    void clear() { destroy_tree_helper(root); }
    bool empty() const { return root == nullptr; }

    //void insert(const Type& key) { insert_helper(f(key)); }

    /*bool find(const Type& key) {
        Type f_x = f(key);
        node_t* x = find_helper(f_x);
        return x != nullptr && x->key == f_x;
    }*/

    //void remove(const Type& key) { erase(f(key)); } //remove_helper(f(key)); }

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


    /// ///////////////////////////////////////

    int _print_t(node_t* tree, int is_left, int offset, int depth, char s[20][255])
    {
        char b[40] = { 0 };
        int width = 10;

        if (!tree) return 0;

        sprintf(b, "(%03d)[%03d]", tree->key, tree->sum);

        int left = _print_t(tree->left, 1, offset, depth + 1, s);
        int right = _print_t(tree->right, 0, offset + left + width, depth + 1, s);

#ifdef COMPACT
        for (int i = 0; i < width; i++)
            s[depth][offset + left + i] = b[i];

        if (depth && is_left) {

            for (int i = 0; i < width + right; i++)
                s[depth - 1][offset + left + width / 2 + i] = '-';

            s[depth - 1][offset + left + width / 2] = '.';

        }
        else if (depth && !is_left) {

            for (int i = 0; i < left + width; i++)
                s[depth - 1][offset - width / 2 + i] = '-';

            s[depth - 1][offset + left + width / 2] = '.';
        }
#else
        for (int i = 0; i < width; i++)
            s[2 * depth][offset + left + i] = b[i];

        if (depth && is_left) {

            for (int i = 0; i < width + right; i++)
                s[2 * depth - 1][offset + left + width / 2 + i] = '-';

            s[2 * depth - 1][offset + left + width / 2] = '+';
            s[2 * depth - 1][offset + left + width + right + width / 2] = '+';

        }
        else if (depth && !is_left) {

            for (int i = 0; i < left + width; i++)
                s[2 * depth - 1][offset - width / 2 + i] = '-';

            s[2 * depth - 1][offset + left + width / 2] = '+';
            s[2 * depth - 1][offset - width / 2 - 1] = '+';
        }
#endif

        return left + width + right;
    }

    void print_t(node_t* tree)
    {
        char s[40][255] = { 0 };
        for (int i = 0; i < 40; i++) {
            sprintf(s[i], "%80s", " ");
        }

        _print_t(tree, 0, 0, 0, s);

        for (int i = 0; i < 40; i++)
            printf("%s\n", s[i]);
    }

    void dump2() {
        print_t(root);
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
            cout << ">>>" << sum << endl;
        }
        //tree.dump();
        //tree.dump2();
    }

    return 0;
}
