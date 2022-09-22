#include <iostream>
#include <string>
#include <cstring>
#include <functional>
#include <algorithm>

using namespace std;

#include <vld\vld.h>


//
//#ifdef NDEBUG
//#define assert(condition, message) 0
//#else
//#define assert(condition, message)\
//   (!(condition)) ?\
//      (std::cerr << "Assertion failed: (" << #condition << "), "\
//      << "function " << __FUNCTION__\
//      << ", file " << __FILE__\
//      << ", line " << __LINE__ << "."\
//      << std::endl << message << std::endl, abort(), 0) : 1
//#endif
//
//template <class Type = long long>
//class splay_tree_t {
//private:
//    splay_tree_t(const splay_tree_t&) = delete;
//    splay_tree_t& operator=(const splay_tree_t&) = delete;
//private:
//    struct node_t {
//        int offset;
//        int weight;
//        struct node_t* parent;
//        struct node_t* left;
//        struct node_t* right;
//
//        /*node_t(const Type& key)
//            : key(key), parent(nullptr), left(nullptr), right(nullptr) { }*/
//
//        node_t(int offset, int weight) 
//            : offset(offset), weight(weight), 
//            parent(nullptr), left(nullptr), right(nullptr) { }
//    };
//protected:
//    node_t* root;
//    const char* m_data;
//    int m_len;
//public:
//    splay_tree_t(const char* data, int len)
//        : root(nullptr), m_data(data), m_len(len) { 
//        root = new node_t(0, len);
//    }
//    ~splay_tree_t() { clear(); }
//private:
//    void destroy_tree_helper(node_t* node) {
//        if (node == nullptr) return;
//        destroy_tree_helper(node->left);
//        destroy_tree_helper(node->right);
//        delete node;
//    }
//
//    void in_order_traverse_helper(node_t* node, function<void(const char* data, int offset, int weight)> f) {
//        if (node == nullptr) return;
//        in_order_traverse_helper(node->left, f);
//        f(m_data, node->offset, node->weight);
//        in_order_traverse_helper(node->right, f);
//    }
//
//    inline void small_rotation(node_t* v) {
//        node_t* parent = v->parent;
//        if (parent == nullptr) { return; }
//
//        node_t* grandparent = v->parent->parent;
//        if (parent->left == v) {
//            node_t* m = v->right;
//            v->right = parent;
//            parent->left = m;
//        } else {
//            node_t* m = v->left;
//            v->left = parent;
//            parent->right = m;
//        }
//        update(parent);
//        update(v);
//        v->parent = grandparent;
//        if (grandparent != nullptr) {
//            if (grandparent->left == parent) {
//                grandparent->left = v;
//            } else {
//                grandparent->right = v;
//            }
//        }
//    }
//
//    inline void big_rotation(node_t* v) {
//        if ((v->parent->left == v && v->parent->parent->left == v->parent)
//            || (v->parent->right == v && v->parent->parent->right == v->parent)) {
//            // Zig-zig
//            small_rotation(v->parent);
//            small_rotation(v);
//        }
//        else {
//            // Zig-zag
//            small_rotation(v);
//            small_rotation(v);
//        }
//    }
//
//    inline void update(node_t* v) {
//        if (v == nullptr) return;
//        //v->sum = v->key
//            //+ (v->left != nullptr ? v->left->sum : 0)
//            //+ (v->right != nullptr ? v->right->sum : 0);
//        v->weight = 
//            + (v->left != nullptr ? v->left->weight : 0)
//            + (v->right != nullptr ? v->right->weight : 0);
//        if (v->left != nullptr) { v->left->parent = v; }
//        if (v->right != nullptr) { v->right->parent = v; }
//    }
//
//
//    inline node_t* splay(node_t* u) {
//        if (u == nullptr) return nullptr;
//        while (u->parent != nullptr) {
//            // No grand parent (we r close to root), do single rotation and stop
//            if (u->parent->parent == nullptr) {
//                small_rotation(u);
//                break;
//            }
//            // Keep pushing 'u' to the root
//            big_rotation(u);
//        }
//        return u;
//    }
//
//    inline node_t* merge(node_t* left, node_t* right) {
//
//        // No left subtree -> new root is right subtree
//        if (left == nullptr) return right;
//
//        // No right subtree -> new root is left subtree
//        if (right == nullptr) return left;
//
//        // Min in right subtree
//        while (right->left != nullptr) { right = right->left; }
//
//        // Move to the root and make assign left subtree as left child
//        right = splay(right);
//        right->left = left;
//
//        // Update sum values 
//        update(right);
//
//        return right;
//    }
//
//    inline pair<node_t*, node_t*> split(node_t* root, int idx) {
//        pair<node_t*, node_t*> result;
//
//        pair<node_t*, node_t*> find_and_root = find(root, idx);
//        root = find_and_root.second;
//        result.second = find_and_root.first;
//        if (result.second == nullptr) {
//            result.first = root;
//            return result;
//        }
//        result.second = splay(result.second);
//        result.first = result.second->left;
//        result.second->left = nullptr;
//
//        // No more parent for left subtree
//        if (result.first != nullptr) {
//            result.first->parent = nullptr;
//        }
//
//        // Update sum value for left/right subtree
//        update(result.first);
//        update(result.second);
//
//        return result;
//    }
//
//    inline pair<node_t*, node_t*> find(node_t* root, int idx) {
//        node_t* v = root;
//        node_t* last = root;
//        node_t* next = nullptr;
//        if (root == nullptr) return make_pair(nullptr, nullptr);
//
//        while (v->left != nullptr && idx > 0) {
//            last = v;
//            if (v->left->weight >= idx) {
//                v = v->left;
//                next = v;
//            } else {
//                next = v;
//                idx -= v->left->weight;
//                v = v->right;                
//            }
//            /*if (v->key >= key
//                && (next == nullptr || v->key < next->key)) {
//                next = v;
//            }
//            last = v;
//            if (v->key == key) { break; }
//            if (v->key < key) { v = v->right; }
//            else { v = v->left; }*/
//        }
//        root = splay(last);
//        return make_pair(next, root);
//    }
//
//    inline char& get(node_t* node, int idx) {
//        while(node->left) {
//            if (node->left->weight >= idx) {
//                node = node->left;
//            } else {
//                idx -= node->left->weight;
//                node = node->right;
//            }
//        }
//
//        char* ptr = const_cast<char*>(m_data);
//        return ptr[node->offset + idx];
//    }
//public:
//    void in_order_traverse(function<void(const char*, int, int)> func) { in_order_traverse_helper(root, func); }
//
//    void clear() { destroy_tree_helper(root); }
//    bool empty() const { return root == nullptr; }
//
//    void insert(const Type& x) {
//        node_t* left = nullptr;
//        node_t* right = nullptr;
//        node_t* new_vertex = nullptr;
//        pair<node_t*, node_t*> leftRight = split(root, x);
//        left = leftRight.first;
//        right = leftRight.second;
//        if (right == nullptr || right->key != x) { new_vertex = new node_t(x); }
//        root = merge(merge(left, new_vertex), right);
//    }
//
//    void remove(const Type& x) {
//        // Use split and merge technique
//        pair<node_t*, node_t*> leftMiddle = split(root, x);
//        node_t* left = leftMiddle.first;
//        node_t* middle = leftMiddle.second;
//        pair<node_t*, node_t*> middleRight = split(middle, x + 1);
//        middle = middleRight.first;
//        node_t* right = middleRight.second;
//        if (middle == nullptr || middle->key != x) {
//            root = merge(merge(left, middle), right);
//        } else {
//            delete middle;
//            root = merge(left, right);
//        }
//    }
//
//    char& operator[](int idx) {
//        return get(root, idx);
//    }
//
//    bool find(int idx) {
//        node_t* left = nullptr;
//        node_t* right = nullptr;
//        pair<node_t*, node_t*> leftRight = split(root, idx);
//        left = leftRight.first;
//        right = leftRight.second;
//        root = merge(left, right);
//
//        return right != nullptr;//&& right->key == x;
//    }
//
//    Type range_sum(const Type& L, const Type& R) {
//        pair<node_t*, node_t*> leftMiddle = split(root, L);
//        node_t* left = leftMiddle.first;
//        node_t* middle = leftMiddle.second;
//        pair<node_t*, node_t*> middleRight = split(middle, R + 1);
//        middle = middleRight.first;
//        node_t* right = middleRight.second;
//        Type ans = 0;
//        // Complete the implementation of sum
//        if (middle != nullptr)
//            ans = middle->sum;
//        middle = merge(middle, right);
//        root = merge(left, middle);
//
//        return ans;
//    }
//};
//


template <class T>
class Rope {
private:
	class node_t {
		friend Rope;

		static int get_weight(node_t* left, node_t* right) {
			return (left != nullptr ? left->weight : 0)
				+ (right != nullptr ? right->weight : 0);
		}

	protected:
		int offset;
		int slice_size;
		int weight;
		node_t* left;
		node_t* right;
		node_t* parent;
	public:
		inline void reset(node_t* left, node_t* right) {
			this->left = left;
			this->right = right;
			weight = get_weight(left, right);
		}

		explicit node_t(int offset, int weight)
			: offset(offset), slice_size(weight), weight(weight),
			left(nullptr), right(nullptr), parent(nullptr) { }

		explicit node_t(node_t* left, node_t* right) 
			: offset(-1), slice_size(0), weight(0), parent(nullptr) { reset(left, right); }
	};

	inline void small_rotation(node_t* v) {
		node_t* parent = v->parent;
		if (parent == nullptr) { return; }

		node_t* grandparent = v->parent->parent;
		if (parent->left == v) {
			node_t* m = v->right;
			v->right = parent;
			parent->left = m;
		}
		else {
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
			} else {
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
		v->weight = v->slice_size//v->key
			+ (v->left != nullptr ? v->left->weight : 0)
			+ (v->right != nullptr ? v->right->weight : 0);
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
protected:
	node_t* root;
	const T* m_data;
	int m_size;
private:
	inline void clear_helper(node_t* node) {
		if (node == nullptr) return;
		clear_helper(node->left);
		clear_helper(node->right);
		delete node;
	}

	inline void in_order_traverse_helper(node_t* node, function<void(const T*, int, int)> f) {
		if (node == nullptr) return;
		in_order_traverse_helper(node->left, f);
		f(m_data, node->offset, node->weight);
		in_order_traverse_helper(node->right, f);
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

		//return new node_t(L, R);
	}

	

	pair<node_t*, node_t*> split(node_t* node, int i) {
		node_t* tree1 = nullptr, * tree2 = nullptr;
		if (node == nullptr) return make_pair(nullptr, nullptr);
		if (node->left) {
			if (node->left->weight >= i) {
				auto res = split(node->left, i);
				tree1 = res.first;
				//node->reset(res.second, node->right);
				//tree2 = node;

				//tree2 = new node_t(res.second, node->right);
				tree2 = merge(res.second, node->right);

				//tree2 = node->reset(res.second, node->right);
			} else {
				auto res = split(node->right, i - node->left->weight);
				//tree1 = new node_t(node->left, res.first);
				tree1 = merge(node->left, res.first);
				////node->reset(node->left, res.first);
				////tree1 = node;
				//tree1 = node->reset(node->left, res.first);
				tree2 = res.second;
			}
		} else {
			//substr(node->data, 0, i);
			size_t w = node->weight;
			tree1 = new node_t(node->offset, i);			
			if ((int)w - i >= 0) {
				tree2 = new node_t(node->offset + i, w - i);
			}
			
		}
		delete node;


		return make_pair(tree1, tree2);
	}


	/*node_t* merge(node_t* L, node_t* R) {
		if (L == nullptr) return R;
		if (R == nullptr) return L;


		return new node_t(L, R);
	}*/

	//node_t* insert(node_t* node, int idx, node_t* insertant) {
	//	if (idx == 0)
	//		return merge(insertant, node);
	//	if (idx >= node->w)
	//		return merge(node, insertant);

	//	auto res = split(node, idx);
	//	auto prefix = res.first;
	//	auto postfix = res.second;

	//	return merge(merge(prefix, insertant), postfix);
	//	//auto it = split(node, insertIndex);
	//	//node_t* tree1 = it.first, * tree3 = it.second;
	//	//node_t* tree2 = new node_t(data, len);
	//	//return merge(merge(tree1, tree2), tree3);
	//}

	T& get(node_t* node, int i) { 
		if (node->left != nullptr) {
			if (node->left->weight >= i) { return get(node->left, i); }
			return get(node->right, i - node->left->weight);
		}
		T* ptr = const_cast<T*>(m_data);
		return ptr[node->offset + i];
	}


	/*node_t* insert(node_t* node, int idx, const T* data, int len) {
		node_t* left = nullptr;
		node_t* right = nullptr;
		node_t* new_vertex = nullptr;
		pair<node_t*, node_t*> leftRight = split(root, idx);
		left = leftRight.first;
		right = leftRight.second;
		if (right == nullptr || right->key != x) { new_vertex = new node_t(x); }
		return merge(merge(left, new_vertex), right);
	}*/
public:
	Rope(const T* data, int len)
		: root(nullptr), m_data(data), m_size(len) {
		
		root = new node_t(0, len);
	}

	~Rope() { clear(); }

	void clear() { clear_helper(root); root = nullptr; }
	void in_order_traverse(function<void(const T*, int, int)> func) { in_order_traverse_helper(root, func); }
	T& operator[](size_t i) { return get(root, i); }

	/*void insert(int idx, const T* data, int len) {
		root = insert(root, idx, data, len);
	}*/

	/*void cut_and_insert2(int i, int j, int k) {
		if (i == 0) {
			auto it = split(root, j - i + 1);
			root = insert(it.first, k, it.second);
			return;
		}

		auto L = split(root, i);
		auto R = split(L.second, j - i + 1);
		root = insert(merge(L.first, R.second), k, R.first);
	}*/

	void cut_and_insert(int i, int j, int k) {
		if (i == 0) {
			if(k == 0) { return; } // substr from start and append to start

			auto R = split(root, j + 1);
			// R.first -> substr
			// R.second -> remaining part of the string
			auto M = split(R.second, k);
			root = merge(merge(M.first, R.first), M.second);
		} else {

			//....i)[i+1......
			auto L = split(root, i);
			auto prefix = L.first;

			auto R = split(L.second, j - i + 1);
			auto substr = R.first;
			auto postfix = R.second;

			auto remaining = merge(prefix, postfix);

			if (k == 0) {
				// L.1]L.2....[
				root = merge(substr, remaining);//merge(L.first, R.second));
			} else {
				auto M = split(remaining, k);

				// Substr + right part of the remaining str
				auto substr_and_R = merge(substr, M.second);
				root = merge(M.first, substr_and_R);
				//root = merge(M.first, merge(R.first, M.second));
			}
		}
	}
};




///////////////////////////////////////////////////////////////////////////////////



int main() {
	string S;
	int q, i, j, k;
	function<void(const char*, int, int)> f = [](const char* data, int offset, int len) {
		if (data == nullptr || offset == -1) return;
		const char* ptr = &data[offset];
		for (size_t i = 0; i < len; ++i)
			cout << ptr[i];
		//cout << " => ";
	};

	cin >> S >> q;

	Rope<char> tree(S.c_str(), S.size());
    //splay_tree_t<char> tree(S.c_str(), S.size());
    //SplayTree<char> tree(S.begin(), S.end());

	tree.in_order_traverse(f);
    cout << endl;
    for (int i = 0; i < S.size(); ++i) {
        cout << tree[i] << " " << endl;
    }
	//cout <<  << endl;
    tree.in_order_traverse(f); cout << endl;
	while (q-- > 0) {
		cin >> i >> j >> k;
		tree.cut_and_insert(i, j, k);
		tree.in_order_traverse(f); cout << endl;
	}
	tree.in_order_traverse(f); cout << endl;
	//while (q-- > 0) {
	//	cin >> i >> j >> k;
 //       SplayTree<char> substr;
 //       if (j + 1 < tree.size()) {
 //           auto postfix = tree.split(j + 1);
 //           substr = tree.split(i);
 //           tree.merge(std::move(postfix));
 //       } else {
 //           substr = tree.split(i);
 //       }
 //       /*K.merge(std::move(substr));
 //       K.merge(std::move(tree));
 //       tree = std::move(K);*/
 //       if (k == 0) {
 //           substr.merge(std::move(tree));
 //           tree = std::move(substr);
 //       } else {
 //           auto K = tree.split(k);
 //           //K.merge(std::move(substr));
 //           substr.merge(std::move(K));
 //           tree.merge(std::move(substr));
 //       }
	//}
 //   for (auto i : tree) { cout << i; }

	return 0;
}