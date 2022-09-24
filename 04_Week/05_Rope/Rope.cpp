#include <iostream>
#include <string>
#include <cstring>
#include <functional>
#include <algorithm>

using namespace std;

#define DEBUG

#ifdef DEBUG
#include <vld\vld.h>
#endif

template <class T>
class rope_array_t {
private:
	class node_t {
		friend rope_array_t;

		static int get_weight(node_t* left, node_t* right) {
			return (left != nullptr ? left->weight : 0)
				+ (right != nullptr ? right->weight : 0);
		}

	protected:
		T data;
		int weight;
		node_t* left;
		node_t* right;
		node_t* parent;
	public:
		explicit node_t(const T& data)
			: data(data), weight(1),
			left(nullptr), right(nullptr), parent(nullptr) { }
	};

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
		v->weight = 1
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

	inline void in_order_traverse_helper(node_t* node, function<void(const T&)> f) {
		if (node == nullptr) return;
		in_order_traverse_helper(node->left, f);
		f(node->data);
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
	}

	pair<node_t*, node_t*> split(node_t* node, int i) {
		// Find node by index, splay, easy split now
		node_t* tree1 = nullptr, * tree2 = nullptr;
		if (node == nullptr) return make_pair(nullptr, nullptr);
		node_t* mid_node = get_node(node, i);
#ifdef DEBUG
		cout << "split: [idx=" << i << "], node=" << (mid_node != nullptr ? mid_node->data : '0') << endl;
#endif
		mid_node = splay(mid_node);
		if(mid_node == nullptr) return make_pair(node, nullptr);
		tree1 = mid_node->left;
		if(tree1 != nullptr)
			tree1->parent = nullptr;

		tree2 = mid_node;
		if (tree2 != nullptr) {
			tree2->left = nullptr;
			tree2->parent = nullptr;
		}
		update(tree1);
		update(tree2);

		return make_pair(tree1, tree2);
	}

	// Explicit keys 
	node_t* get_node(node_t* node, int i) {
		int idx = 0;
		while (node != nullptr) {
			if (node->left != nullptr) {
				if (node->left->weight > i) {
					node = node->left;
				} else if(node->left->weight == i) {
					return node;
				} else {
					i = i - node->left->weight - 1;
					node = node->right;
				}
			} else if(node->right != nullptr) {
				if (node->weight - 1 == i + node->right->weight)
					return node;

				node = node->right;
				i = i - 1;
			} else if(i == 0) {
				return node;
			} else {
				return nullptr;
			}
		}
		return nullptr;
	}

	T& get(node_t* node, int i) { 
		node = get_node(node, i);
		if (node == nullptr) throw std::out_of_range("Index out of bounds");
		return node->data;
	}
public:
	rope_array_t(const T* data, int len)
		: root(nullptr), m_data(data), m_size(len) {
		
		for (int i = 0; i < len; ++i) {
			root = splay(merge(root, new node_t(data[i])));
		}
	}

	~rope_array_t() { clear(); }

	void clear() { clear_helper(root); root = nullptr; }
	void in_order_traverse(function<void(const T&)> func) { in_order_traverse_helper(root, func); }
	T& operator[](size_t i) { return get(root, i); }

	void cut_and_insert(int i, int j, int k) {
		if (i == 0) {
			if(k == 0) { return; } // substr from 0 index and prepend result

			auto R = split(root, j + 1);
			// R.first -> substr
			// R.second -> remaining part of the string
			auto M = split(R.second, k);
			root = merge(merge(M.first, R.first), M.second);
		} else {

			//....i)[i+1......
			auto L = split(root, i);
			auto prefix = L.first;

#ifdef DEBUG
			std::function<void(const T&)> f = [](const T& data) { cout << data << "->"; };
			cout << endl << "prefix: " << endl;
			root = prefix;
			in_order_traverse(f);
#endif

			auto R = split(L.second, j - i + 1);
			auto substr = R.first;
			auto postfix = R.second;

#ifdef DEBUG
			cout << endl << "substr: " << endl;
			root = substr;
			in_order_traverse(f);

			cout << endl << "postfix: " << endl;
			root = postfix;
			in_order_traverse(f);
#endif

			auto remaining = merge(prefix, postfix);

#ifdef DEBUG
			cout << endl << "remainder (prefix+postfix): " << endl;
			root = remaining;
			in_order_traverse(f);
#endif

			if (k == 0) {
				// L.1]L.2....[
				root = merge(substr, remaining);//merge(L.first, R.second));
			} else {
				auto M = split(remaining, k);

#ifdef DEBUG
				cout << endl << "M.L split by k: " << endl;
				root = M.first;
				in_order_traverse(f);

				cout << endl << "M.R split by k: " << endl;
				root = M.second;
				in_order_traverse(f);
#endif

				// Substr + right part of the remaining str
				auto substr_and_R = merge(substr, M.second);

#ifdef DEBUG
				cout << endl << "substr + M.R: " << endl;
				root = substr_and_R;
				in_order_traverse(f);
#endif

				root = merge(M.first, substr_and_R);
			}
		}
	}
};


int main() {
	string S;
	int q, i, j, k;
	function<void(const char&)> f = [](const char& data) {
#ifdef DEBUG
		cout << data << " ";
#else 
		cout << data;
#endif
	};

	cin >> S >> q;

	rope_array_t<char> tree(S.c_str(), S.size());
#ifdef DEBUG
	tree.in_order_traverse(f);
    cout << endl;
    for (int i = 0; i < S.size(); ++i) {
        cout << i << " " << tree[i] << " " << endl;
    }
	//cout <<  << endl;
    tree.in_order_traverse(f); cout << endl;
#endif
	while (q-- > 0) {
		cin >> i >> j >> k;
		tree.cut_and_insert(i, j, k);
#ifdef DEBUG
		tree.in_order_traverse(f); cout << endl;
#endif
	}
	tree.in_order_traverse(f); cout << endl;

	return 0;
}