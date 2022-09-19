//#include <iostream>
//#include <string>
//#include <cstring>
//#include <functional>
//#include <algorithm>
//
//using namespace std;
//
//#include <vld\vld.h>
//
//template <class T>
//class Rope {
//private:
//    struct node_t {
//        T data;
//        int w;
//        node_t* left;
//        node_t* right;
//        node_t* parent;
//
//        explicit node_t(const T& data, int len, 
//            node_t* left = nullptr, node_t* right = nullptr, node_t* parent = nullptr)
//            : data(data), w(len),
//            left(left), right(right), parent(parent) { }
//
//        //virtual ~node_t() { if (data != nullptr) delete[] data; }
//    };
//
//    inline void set_parent(node_t* child, node_t* parent) { if (child != nullptr) child->parent = parent; }
//    inline void keep_parent(node_t* node) { set_parent(node->left, node); set_parent(node->right, node); }
//    inline void set_weight(node_t* node) {
//        node->w = 1
//            + (node->left != nullptr ? node->left->w : 0)
//            + (node->right != nullptr ? node->right->w : 0);
//    }
//    inline int get_node_idx(node_t* node) const { return node->w - 1 - (node->right != nullptr ? node->right->w : 0); }
//
//    inline void rotate(node_t* P, node_t* C) {
//        auto G = P->parent;
//        if (G != nullptr) {
//            if (G->left == P) {
//                G->left = C;
//            } else {
//                G->right = C;
//            }
//        }
//
//        if (P->left == C) {
//            P->left = C->right;
//            C->right = P;
//        } else {
//            P->right = C->left;
//            C->left = P;
//        }
//
//        keep_parent(P);
//        keep_parent(C);
//        C->parent = G;
//        set_weight(P);
//        set_weight(C);
//    }
//
//    inline node_t* soft_balance(node_t* node) { return find(root, (root->w - 1) / 2); }
//    inline node_t* splay(node_t* node) {
//        if (node == nullptr) return nullptr;
//        while (node->parent != nullptr) {
//            auto P = node->parent;
//            auto G = P->parent;
//            if (G == nullptr) {
//                rotate(P, node);
//                break;
//            } else {
//                // Zig-Zig/Zag-Zag
//                if (G->left == P && P->left == node) {
//                    rotate(G, P);
//                    rotate(P, node);
//                } else {
//                    rotate(P, node);
//                    rotate(G, node);
//                }
//            }
//        }
//        root = node;
//        return node;
//    }
//    inline node_t* find(node_t* node, int idx) {
//        while (node != nullptr) {
//            int node_idx = get_node_idx(node);
//            if (idx == node_idx) return splay(node);
//            if (idx < node_idx && node->left != nullptr) {
//                node = node->left;
//            } else if (idx > node_idx && node->right != nullptr) {
//                idx -= node_idx + 1;
//                node = node->right;
//            } else {
//                return splay(node);
//            }
//        }
//        return nullptr;
//    }
//
//    inline pair<node_t*, node_t*> split(node_t* node, int idx) {
//        if (node == nullptr) return make_pair(nullptr, nullptr);
//
//        node = find(node, idx);
//        if (node == nullptr) return make_pair(nullptr, nullptr);
//
//        int node_idx = get_node_idx(node);
//        if (node_idx == idx) {
//            auto left = node->left;
//            if (left != nullptr) {
//                node->left = node->parent = nullptr;
//                node->w -= left->w;
//            }
//            return make_pair(left, node);
//        }
//
//        if (node_idx < idx) { return make_pair(node, nullptr); }
//
//        return make_pair(nullptr, nullptr);
//    }
//
//    inline node_t* merge(node_t* L, node_t* R) {
//        if (L == nullptr) return R;
//        if (R == nullptr) return find(L, 0);
//
//        R = find(R, 0);
//        R->left = L;
//        L->parent = R;
//        R->w += L->w;
//
//        return find(R, 0);
//    }
//
//    inline void clear_helper(node_t* node) {
//        /*if (node == nullptr) return;
//        clear_helper(node->left);
//        clear_helper(node->right);
//        delete node;*/
//    }
//    
//    inline void in_order_traverse_helper(node_t* node, function<void(const T&)> f) {
//		if (node == nullptr) return;
//		in_order_traverse_helper(node->left, f);
//		f(node->data);
//		in_order_traverse_helper(node->right, f);
//	}
//protected:
//    node_t* root;
//public:
//    Rope(const T* data, size_t len)
//        : root(nullptr) {
//        
//        if (len > 0) {
//
//            root = new node_t(data[0], 1);
//            for (size_t i = 1; i < len; ++i) {
//                root = new node_t(data[i], 1, root);
//                set_parent(root->left, root);
//                set_weight(root);
//            }
//            root = soft_balance(root);
//        }
//    }
//    ~Rope() { clear_helper(root); }
//    
//    T& operator[](int idx) {
//        auto node = find(root, idx);
//        if (node == nullptr) throw std::out_of_range("Index out of bounds");
//        return node->data;
//    }
//
//    inline void cut_and_insert(int i, int j, int k) {
//        auto T1 = split(root, i);
//        auto T2 = split(T1.second, j - i + 1);
//        root = merge(T1.first, T2.second);
//        auto T3 = split(root, k);
//        root = merge(T2.first, T3.second);
//        root = soft_balance(merge(T3.first, root));
//    }
//    inline void in_order_traverse(function<void(const T&)> func) { in_order_traverse_helper(root, func); }
//};
//int main() {
//    function<void(char)> f = [](char data) {
//        cout << data << " ";
//    };
//
//    int Q, i, j, k;
//    string S;
//
//    
//    cin >> S >> Q;
//    Rope<char> tree(S.c_str(), S.size());
//
//    while (Q-- > 0) {
//        cin >> i >> j >> k;
//        tree.cut_and_insert(i, j, k);
//    }
//    tree.in_order_traverse(f); cout << endl;
//    return 0;
//}