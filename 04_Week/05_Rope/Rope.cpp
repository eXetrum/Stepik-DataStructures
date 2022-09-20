#include <iostream>
#include <string>
#include <cstring>
#include <functional>
#include <algorithm>

using namespace std;

#include <vld\vld.h>


template <class T>
class Rope {
	const int MAX_DATA_LENGTH = 32;
private:
	class node_t {
		friend Rope;
		static int get_weight(node_t* left, node_t* right) {
			return (left != nullptr ? left->w : 0)
				+ (right != nullptr ? right->w : 0);
		}

	protected:
		T* data;
		int w;
		int h;
		node_t* left;
		node_t* right;
	public:
		inline void reset(node_t* left, node_t* right) {
			if (data != nullptr) {
				delete[] data;
				data = nullptr;
			}
			this->left = left;
			this->right = right;
			w = get_weight(left, right);
		}

		explicit node_t(T* data, int len, node_t* left=nullptr, node_t* right=nullptr)
			: data(data), w(len), h(1), 
			left(left), right(right) { }

		//explicit node_t(node_t* left, node_t* right) : data(nullptr), w(0) { reset(left, right); }

		virtual ~node_t() { if (data != nullptr) delete[] data; }
	};

	static inline T* slice(const T* data, int start, int len) {
		cout << "len=" << len << endl;
		if (len <= 0) return nullptr;
		T* chunk = new T[len];
		T* ptr = (T*)memcpy(chunk, &data[start], sizeof(T) * len);
		return ptr;
	}

	static inline int get_weight(node_t* node) { return node != nullptr ? node->w : 0; }
	static inline int get_height(node_t* node) { return node != nullptr ? node->h : 0; }
	static inline int get_balance_factor(node_t* node) { return node != nullptr ? get_height(node->right) - get_height(node->left) : 0; }
	static inline void update_height(node_t* node) { node->h = std::max(get_height(node->left), get_height(node->right)) + 1; }
	static inline void update_weight(node_t* node) { node->w = get_weight(node->left) + get_weight(node->right); }
	static inline void update(node_t* node) { update_height(node); update_weight(node); }
protected:
	node_t* root;
private:
	inline void clear_helper(node_t* node) {
		if (node == nullptr) return;
		clear_helper(node->left);
		clear_helper(node->right);
		delete node;
	}

	inline void in_order_traverse_helper(node_t* node, function<void(const T*, size_t)> f) {
		if (node == nullptr) return;
		in_order_traverse_helper(node->left, f);
		f(node->data, node->w);
		in_order_traverse_helper(node->right, f);
	}

	static inline node_t* rotate_right(node_t* node) {
		node_t* top_node = node->left;
		if (top_node != nullptr) {
			node->left = top_node->right;
			top_node->right = node;
			update(node);
			update(top_node);
			return top_node;
		}
		return node;
	}
	static inline node_t* rotate_left(node_t* node) {
		node_t* top_node = node->right;
		if (top_node != nullptr) {
			node->right = top_node->left;
			top_node->left = node;
			update(node);
			update(top_node);
			return top_node;
		}
		return node;
	}


	static inline node_t* balance(node_t* node) {
		if (node == nullptr) return nullptr;
		update(node);
		int factor = get_balance_factor(node);
		while (abs(factor) > 1) {
			if (factor > 1) {
				if (get_balance_factor(node->right) < 0) { node->right = rotate_right(node->right); }
				node = rotate_left(node);
			} else if (factor < -1) {
				if (get_balance_factor(node->left) > 0) { node->left = rotate_left(node->left); }
				node = rotate_right(node);
			}
			factor = get_balance_factor(node);
		}
		return node;
	}

	static inline T find(node_t* node, int idx) {
		if (node->left != nullptr) {
			if (node->left->w < idx) {
				return find(node->right, idx - node->left->w);
			}
			return find(node->left, idx);
		}
		return node->data[idx];
	}

	static inline node_t* merge(node_t* L, node_t* R) {
		if (L == nullptr) return R;
		if (R == nullptr) return L;

		//int w = get_weight(L) + get_weight(R);
		//if(w <= MAX_DATA_LENGTH) 
			//return new node_t(join_to_single(L, R))
		return new node_t(nullptr, get_weight(L) + get_weight(R), L, R);
	}

	static inline pair<node_t*, node_t*> split(node_t* node, int idx) {
		if (node == nullptr) return make_pair(nullptr, nullptr);
		if (node->left != nullptr) {
			if (node->left->w >= idx) {
				auto res = split(node->left, idx);
				auto R = node->right;
				
				delete node;
				update(res.first);
				update(res.second);
				update(R);
				auto Q = merge(res.second, R);
				update(Q);
				return make_pair(res.first, Q);
			}
			auto res = split(node->right, idx - node->left->w);
			auto L = node->left;
			
			delete node;
			update(res.first);
			update(res.second);
			update(L);
			auto Q = merge(L, res.first);
			update(Q);

			return make_pair(Q, res.second);
		}
		
		int w = node->w;
		auto prefix  = slice(node->data, 0, idx);
		auto postfix = slice(node->data, idx, w - idx);
		
		delete node;
		node_t* L = nullptr, * R = nullptr;

		if(prefix != nullptr) L = new node_t(prefix, idx);
		if(postfix != nullptr) R = new node_t(postfix, w - idx);
		return make_pair(L, R);
	}

	

	//pair<node_t*, node_t*> split(node_t* node, size_t i) {
	//	node_t* tree1 = nullptr, * tree2 = nullptr;
	//	if (node == nullptr) return make_pair(nullptr, nullptr);
	//	if (node->left) {
	//		if (node->left->w >= i) {
	//			auto res = split(node->left, i);
	//			tree1 = res.first;
	//			//node->reset(res.second, node->right);
	//			//tree2 = node;

	//			tree2 = new node_t(res.second, node->right);
	//			tree2->parent = nullptr;

	//			//tree2 = node->reset(res.second, node->right);
	//		} else {
	//			auto res = split(node->right, i - node->left->w);
	//			tree1 = new node_t(node->left, res.first);
	//			////node->reset(node->left, res.first);
	//			////tree1 = node;
	//			//tree1 = node->reset(node->left, res.first);
	//			tree2 = res.second;

	//			
	//		}
	//	} else {
	//		//substr(node->data, 0, i);
	//		size_t w = node->w;
	//		T* ptr = node->data;
	//		
	//		
	//		tree1 = new node_t(slice(ptr, 0, i), i);
	//		//tree2 = new node_t(node->s.substr(i, node->s.size()), node->s.size() - i);
	//		if ((int)w - i >= 0) {
	//			/*node->data = &node->data[i];
	//			node->left = node->right = nullptr;
	//			node->w = w - i;*/
	//			tree2 = new node_t(slice(ptr, i, w - i), w - i);
	//		}

	//		////node->w = i;
	//		////tree1 = node;// new node_t(slice(ptr, 0, i), i);
	//		
	//	}
	//	delete node;


	//	return make_pair(tree1, tree2);
	//}


	/*node_t* merge(node_t* L, node_t* R) {
		if (L == nullptr) return R;
		if (R == nullptr) return L;


		return new node_t(L, R);
	}*/

	node_t* insert(node_t* node, int idx, node_t* insertant) {
		if (idx == 0)
			return merge(insertant, node);
		if (idx >= node->w)
			return merge(node, insertant);

		auto res = split(node, idx);
		auto prefix = res.first;
		auto postfix = res.second;

		return merge(merge(prefix, insertant), postfix);
		//auto it = split(node, insertIndex);
		//node_t* tree1 = it.first, * tree3 = it.second;
		//node_t* tree2 = new node_t(data, len);
		//return merge(merge(tree1, tree2), tree3);
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

	/*void insert(int idx, const T* data, size_t len) {
		root = insert(root, idx, data, len);
	}*/

	void cut_and_insert2(int i, int j, int k) {
		if (i == 0) {
			auto it = split(root, j - i + 1);
			root = insert(it.first, k, it.second);
			return;
		}

		auto L = split(root, i);
		auto R = split(L.second, j - i + 1);
		root = insert(merge(L.first, R.second), k, R.first);
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
		tree.cut_and_insert2(i, j, k);
		tree.in_order_traverse(f); cout << endl;
	}
	tree.in_order_traverse(f); cout << endl;
	return 0;
}