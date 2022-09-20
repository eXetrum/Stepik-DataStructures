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

		static int get_weight(node_t* left, node_t* right) {
			return (left != nullptr ? left->weight : 0)
				+ (right != nullptr ? right->weight : 0);
		}

	protected:
		int offset;
		int weight;
		node_t* left;
		node_t* right;
	public:
		inline void reset(node_t* left, node_t* right) {
			this->left = left;
			this->right = right;
			weight = get_weight(left, right);
		}

		explicit node_t(int offset, int weight)
			: offset(offset), weight(weight),
			left(nullptr), right(nullptr) { }

		explicit node_t(node_t* left, node_t* right) 
			: offset(-1), weight(0) { reset(left, right); }
	};
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

	inline node_t* merge(node_t* L, node_t* R) {
		if (L == nullptr) return R;
		if (R == nullptr) return L;

		return new node_t(L, R);
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

				tree2 = new node_t(res.second, node->right);

				//tree2 = node->reset(res.second, node->right);
			} else {
				auto res = split(node->right, i - node->left->weight);
				tree1 = new node_t(node->left, res.first);
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
			if (node->left->w >= i) { return get(node->left, i); }
			return get(node->right, i - node->left->w);
		}
		return m_data[node->offset + i];
	}
public:
	Rope(const T* data, int len)
		: root(nullptr), m_data(data), m_size(len) {
		
		root = new node_t(0, len);
	}

	~Rope() { clear(); }

	void clear() { clear_helper(root); root = nullptr; }
	void in_order_traverse(function<void(const T*, int, int)> func) { in_order_traverse_helper(root, func); }
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
	function<void(const char*, int, int)> f = [](const char* data, int offset, int len) {
		if (data == nullptr || offset == -1) return;
		const char* ptr = &data[offset];
		for (size_t i = 0; i < len; ++i)
			cout << ptr[i];
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