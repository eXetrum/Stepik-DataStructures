#include <iostream>
#include <string>
#include <functional>
#include <algorithm>
#include <iomanip>
using namespace std;


#include <vld/vld.h>


template <class Type=string>
class Rope {
private:
    struct node_t {
        Type s;
		size_t w;
		int h;        
        node_t* left;
        node_t* right;

		static int height(node_t* t) {
			if (t == nullptr) return 0;
			return t->h;
		}

		node_t* reset(const Type& s, size_t weight) {
			this->s = s;
			w = weight;
			h = 1;
			left = right = nullptr;
			
			return this;
		}

		node_t* reset(node_t* left, node_t* right) {
			s = Type();
			w = (left != nullptr ? left->w : 0) + (right != nullptr ? right->w : 0);
			h = max(height(left), height(right)) + 1;
			this->left = left;
			this->right = right;

			return this;
		}

        node_t(const Type& s, size_t weight)
            : s(s), w(weight), h(1), 
			left(nullptr), right(nullptr) { }

        node_t(node_t* left, node_t* right)
            : s(Type()), 
			w((left != nullptr ? left->w : 0) + (right != nullptr ? right->w : 0)), 
			h(max(height(left), height(right)) + 1),
            left(left), right(right) { }
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

    void in_order_traverse_helper(node_t* node, function<void(const Type&)> f) {
        if (node == nullptr) return;
        in_order_traverse_helper(node->left, f);
        f(node->s);
        in_order_traverse_helper(node->right, f);
    }

    void pre_order_traverse_helper(node_t* node, function<void(const Type&)> f) {
        if (node == nullptr) return;
        f(node->s);
        pre_order_traverse_helper(node->left, f);
        pre_order_traverse_helper(node->right, f);
    }

    void helper_post_order_traverse(node_t* node, function<void(const Type&)> f) {
        if (node == nullptr) return;
        helper_post_order_traverse(node->left, f);
        helper_post_order_traverse(node->right, f);
        f(node->s);
    }

	
	////////////////////
	node_t* singleLeftRotate(node_t* t) {

		node_t* p = t->right;
		t->right = p->left;
		p->left = t;

		/* Update heights */
		t->h = 1 + max(height(t->left), height(t->right));
		p->h = 1 + max(height(p->left), height(p->right));
		return t;
	}

	node_t* singleRightRotate(node_t* t) {
		node_t* p = t->left;
		p->left = t->right;
		t->right = p;
		t->h = 1 + max(height(t->left), height(t->right));
		p->h = 1 + max(height(p->left), height(p->right));
		return t;
	}

	node_t* doubleLeftRotate(node_t* t) {
		t->right = singleRightRotate(t->right);
		return singleLeftRotate(t);
	}

	node_t* doubleRightRotate(node_t* t) {
		t->left = singleLeftRotate(t->left);
		return singleRightRotate(t);
	}


	// Retunrs subtree height
	int height(node_t* t) const {
		if (t == nullptr) return 0;
		return t->h;
	}

	// Returns balance factor for specified node
	int getBalance(node_t* t) const {
		if (t == nullptr) return 0;
		return height(t->left) - height(t->right);
	}

	void dump_tree(node_t* t, int depth=1) {
		if (t == nullptr) return;

		dump_tree(t->left, depth + 1);
		cout << setfill('.') << setw(depth * 4) << " " << "data=" << t->s << ", w=" << t->w << ", h=" << t->h << endl;

		
		dump_tree(t->right, depth + 1);
	}

	node_t* rebalance(node_t* t) {
		if (t == nullptr) return t;
		cout << "before balance: " << endl;
		dump_tree(root);
		cout << endl << endl;
		t->h = std::max(height(t->left), height(t->right)) + 1;
		// Balance subtree
		if (getBalance(t) == 2) {
			cout << "rebalance +2" << endl;
			if (getBalance(t->left) > getBalance(t->right)) {
				t = singleLeftRotate(t);
			} else {
				t = singleRightRotate(t);
			}
		} else if (getBalance(t) == -2) {
			cout << "rebalance -2" << endl;
			if (getBalance(t->right) > getBalance(t->left)) {
				t = singleRightRotate(t);
			} else {
				t = singleLeftRotate(t);
			}
		}
		cout << "after balance: " << endl;
		dump_tree(root);
		cout << endl << endl;
		return t;
	}

	pair<node_t*, node_t*> split(node_t* node, int i) {
		node_t* tree1 = nullptr, *tree2 = nullptr;
		if (node == nullptr) return make_pair(nullptr, nullptr);
		if (node->left) {
			if (node->left->w >= i) {
				auto res = split(node->left, i);
				tree1 = res.first;
				//tree2 = new node_t(res.second, node->right);
				tree2 = node->reset(res.second, node->right);

				//tree1 = rebalance(tree1);
				//tree2 = rebalance(tree2);

			} else {
				auto res = split(node->right, i - node->left->w);
				//tree1 = new node_t(node->left, res.first);
				tree1 = node->reset(node->left, res.first);
				tree2 = res.second;

				//tree1 = rebalance(tree1);
				//tree2 = rebalance(tree2);
			}
		} else {
			
			//tree1 = new node_t(node->s.substr(0, i), i);
			
			tree2 = new node_t(node->s.substr(i, node->s.size()), node->s.size() - i);
			tree1 = node->reset(node->s.substr(0, i), i);

			//tree1 = node;
			
			//tree1 = rebalance(tree1);
			//tree2 = rebalance(tree2);
		}

		//delete node;

		return make_pair(tree1, tree2);
	}

	node_t* merge(node_t* L, node_t* R) {
		//node_t* n = new node_t(L, R);
		//return n;
		if (L == nullptr) return R;
		if (R == nullptr) return L;
		return new node_t(L, R);
	}

	node_t* insert(node_t* node, int insertIndex, const Type& s) {
		auto it = split(node, insertIndex);
		node_t* tree1 = it.first, * tree3 = it.second;
		node_t* tree2 = new node_t(s, s.size());
		return merge(merge(tree1, tree2), tree3);
	}

    node_t* build_tree(const string& s) {
		//node_t* cur = nullptr;
		//for (size_t i = 0; i < s.size(); ++i) {
		//	insert(i, s[i]);
		//	//cur = merge(cur, new node_t(s[i], 1));
		//	//root = rebalance(root);
		//}
		//return root;
		return new node_t(s, s.size());
		
   //     for (size_t i = 0; i < s.size(); ++i) {
   //         cur = merge(cur, new node_t(s[i], 1));
			//// Balance subtree
			//if (getBalance(cur) == 2) {
			//	if (getBalance(cur->left) == 1) {
			//		cur = singleLeftRotate(cur);
			//	} else {
			//		cur = doubleLeftRotate(cur);
			//	}
			//}

			//if (getBalance(cur) == -2) {
			//	if (getBalance(cur->right) == -1) {
			//		cur = singleRightRotate(cur);
			//	} else {
			//		cur = doubleRightRotate(cur); 
			//	}
			//}
   //     }
   //     return cur;
    }

	char& get(node_t* node, int i) {
		if (node->left != nullptr) {
			if (node->left->w >= i) { return get(node->left, i); }
			return get(node->right, i - node->left->w);
		}
		return node->s[i];
	}
public:
    Rope(const string& s) 
        : root(nullptr) { root = build_tree(s); }

    ~Rope() { clear(); }
    
    bool empty() const { return root == nullptr; }
    void clear() { clear_helper(root); root = nullptr; }
    void in_order_traverse(function<void(const Type&)> func) { in_order_traverse_helper(root, func); }
    void pre_order_traverse(function<void(const Type&)> func) { pre_order_traverse_helper(root, func); }
    void post_order_traverse(function<void(const Type&)> func) { helper_post_order_traverse(root, func); }

	char& operator[](size_t i) { return get(root, i); }
	
	void insert(int insertIndex, const Type& s) {
		root = insert(root, insertIndex, s);
	}

	void cut_and_insert(int i, int j, int k) {
		//....i)[i+1......
		auto L = split(root, i);
		
		
		auto R = split(L.second, j - i + 1);

		if (k == 0) {
			// L.1]L.2....[
			root = merge(R.first, merge(L.first, R.second));
		} else {
			auto M = split(merge(L.first, R.second), k);


			root = merge(M.first, merge(R.first, M.second));
			//root = merge(R.first, );
		}

		//root = R.first;
		//clear_helper(R.second);
		
		
	}
};


int main() {
    string S;
    int q, i, j, k;
    function<void(string)> f = [](const string& k) {
		cout << k;
    };

    cin >> S >> q;

	Rope<> tree(S);
	tree.in_order_traverse(f);
	while (q-- > 0) {
		cin >> i >> j >> k;
		tree.cut_and_insert(i, j, k);
		tree.in_order_traverse(f); cout << endl;
	}
    return 0;
}