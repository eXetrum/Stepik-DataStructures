#include <iostream>
#include <string>
#include <cstring>
#include <functional>
#include <algorithm>

using namespace std;

#include <vld\vld.h>


template <class T>
class Rope {
private:
	class node_t {
		friend Rope;

	protected:
		T* data;
		int w;
		int h;
		node_t* left;
		node_t* right;
		node_t* parent;
	public:
		inline void reset(node_t* left, node_t* right, node_t* parent=nullptr) {
			if (data != nullptr) {
				delete[] data;
				data = nullptr;
			}
			w = (left != nullptr ? left->w : 0)
				+ (right != nullptr ? right->w : 0);

			this->left = left;
			this->right = right;
			this->parent = parent;
		}

		explicit node_t(T* data, int len)
			: data(data), w(len), left(nullptr), right(nullptr) { }

		explicit node_t(node_t* left, node_t* right, node_t* parent=nullptr) : data(nullptr), w(0) { reset(left, right, parent); }

		virtual ~node_t() { if (data != nullptr) delete[] data; }
	};

protected:
	node_t* root;
private:
	void clear_helper(node_t* node) {
		if (node == nullptr) return;
		clear_helper(node->left);
		clear_helper(node->right);
		delete node;
	}

	void in_order_traverse_helper(node_t* node, function<void(const T*, size_t)> f) {
		if (node == nullptr) return;
		in_order_traverse_helper(node->left, f);
		f(node->data, node->w);
		in_order_traverse_helper(node->right, f);
	}

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
		}
		else {
			// Zig-zag
			small_rotation(v);
			small_rotation(v);
		}
	}

	inline void update(node_t* v) {
		if (v == nullptr) return;
		v->w = (v->data != nullptr ? v->w : 0)
			+ (v->left != nullptr ? v->left->w : 0)
			+ (v->right != nullptr ? v->right->w : 0);
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

	inline node_t* merge(node_t* L, node_t* R) {

		// No left subtree -> new root is right subtree
		if (L == nullptr) return R;

		// No right subtree -> new root is left subtree
		if (R == nullptr) return L;

		// Min in right subtree
		while (R->left != nullptr) { R = R->left; }

		// Move to the root and make assign left subtree as left child
		R = splay(R);
		R->left = L;

		// Update sum values 
		update(R);

		return R;
	}

	inline static T* slice(const T* data, size_t start, size_t len) {
		if (len == 0) return nullptr;
		T* chunk = new T[len];
		
		memset(chunk, 0, sizeof(T) * len);
		T* ptr = (T*)memcpy(chunk, &data[start], sizeof(T) * len);
		return ptr;
	}

	inline void keep_parent(node_t* P, node_t* C) { if (C != nullptr) C->parent = P; }

	pair<node_t*, node_t*> split(node_t* node, size_t i) {
		node_t* tree1 = nullptr, * tree2 = nullptr;
		if (node == nullptr) return make_pair(nullptr, nullptr);
		if (node->left) {
			if (node->left->w >= i) {
				auto res = split(node->left, i);
				tree1 = res.first;
				//node->reset(res.second, node->right);
				//tree2 = node;

				tree2 = new node_t(res.second, node->right);
				tree2->parent = nullptr;

				//tree2 = node->reset(res.second, node->right);
			} else {
				auto res = split(node->right, i - node->left->w);
				tree1 = new node_t(node->left, res.first);
				////node->reset(node->left, res.first);
				////tree1 = node;
				//tree1 = node->reset(node->left, res.first);
				tree2 = res.second;

				
			}
		} else {
			//substr(node->data, 0, i);
			size_t w = node->w;
			T* ptr = node->data;
			
			
			tree1 = new node_t(slice(ptr, 0, i), i);
			//tree2 = new node_t(node->s.substr(i, node->s.size()), node->s.size() - i);
			if ((int)w - i >= 0) {
				/*node->data = &node->data[i];
				node->left = node->right = nullptr;
				node->w = w - i;*/
				tree2 = new node_t(slice(ptr, i, w - i), w - i);
			}

			////node->w = i;
			////tree1 = node;// new node_t(slice(ptr, 0, i), i);
			
		}
		delete node;

		if (tree1 != nullptr) {
			keep_parent(tree1, tree1->left);
			keep_parent(tree1, tree1->right);
		}
		if (tree2 != nullptr) {
			keep_parent(tree2, tree2->left);
			keep_parent(tree2, tree2->right);
		}

		return make_pair(tree1, tree2);
	}


	/*node_t* merge(node_t* L, node_t* R) {
		if (L == nullptr) return R;
		if (R == nullptr) return L;


		return new node_t(L, R);
	}*/

	node_t* insert(node_t* node, int insertIndex, const T* data, size_t len) {
		auto it = split(node, insertIndex);
		node_t* tree1 = it.first, * tree3 = it.second;
		node_t* tree2 = new node_t(data, len);
		return merge(merge(tree1, tree2), tree3);
	}

	node_t* build_tree(const T* data, size_t len) {
		return new node_t(slice(data, 0, len), len);
	}

	T& get(node_t* node, int i) { 
		if (node->left != nullptr) {
			if (node->left->w >= i) { return get(node->left, i); }
			return get(node->right, i - node->left->w);
		}
		return node->data[i];
	}
public:
	Rope(const T* data, size_t len)
		: root(nullptr) {
		root = build_tree(data, len);
	}

	~Rope() { clear(); }

	void clear() { clear_helper(root); root = nullptr; }
	void in_order_traverse(function<void(const T*, size_t len)> func) { in_order_traverse_helper(root, func); }
	T& operator[](size_t i) { return get(root, i); }

	void insert(int insertIndex, const T* data, size_t len) {
		root = insert(root, insertIndex, data, len);
	}

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

			auto R = split(L.second, j - i + 1);

			if (k == 0) {
				// L.1]L.2....[
				root = merge(R.first, merge(L.first, R.second));
			}
			else {
				auto M = split(merge(L.first, R.second), k);


				root = merge(M.first, merge(R.first, M.second));
			}
		}
	}
};





int main() {
	string S;
	int q, i, j, k;
	function<void(const char*, size_t)> f = [](const char* data, size_t len) {
		if (data == nullptr) return;
		for(size_t i = 0; i < len; ++i)
			cout << data[i];
		//cout << " => ";
	};

	cin >> S >> q;

	Rope<char> tree(S.c_str(), S.size());
	tree.in_order_traverse(f);
	while (q-- > 0) {
		cin >> i >> j >> k;
		tree.cut_and_insert(i, j, k);
		tree.in_order_traverse(f); cout << endl;
	}
	tree.in_order_traverse(f); cout << endl;
	return 0;
}