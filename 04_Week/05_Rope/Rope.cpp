#include <iostream>
#include <string>
#include <functional>
#include <algorithm>
using namespace std;


#include <vld/vld.h>

template <class T>
class AVL {
private:
	struct node {
		T data;
		node* left;
		node* right;
		int height;
	};

	// to delete all nodes
	void makeEmpty(node* t) {
		if (t == NULL) return;
		// Recursive call for left and right subtree
		makeEmpty(t->left);
		makeEmpty(t->right);
		// Remove node
		delete t;
	}

	node* singleLeftRotate(node*& t) {
		node* p = t->right;
		node* temp = p->left;

		/* Rotation */
		p->left = t;
		t->right = temp;

		/* Update heights */
		t->height = 1 + max(this->height(t->left), this->height(t->right));
		p->height = 1 + max(this->height(p->left), this->height(p->right));

		/* Return changed node */
		return p;
	}

	node* singleRightRotate(node*& t) {
		node* x = t->left;
		node* temp = x->right;

		/* Rotation */
		x->right = t;
		t->left = temp;

		/* Update height */
		t->height = 1 + max(this->height(t->left), this->height(t->right));
		x->height = 1 + max(this->height(x->left), this->height(x->right));

		/* Save changes */
		return x;
	}

	node* doubleLeftRotate(node*& t) {
		t->right = singleRightRotate(t->right);
		return singleLeftRotate(t);
	}

	node* doubleRightRotate(node*& t) {
		t->left = singleLeftRotate(t->left);
		return singleRightRotate(t);
	}

	node* findMin(node* t) const {
		if (t == NULL) return NULL;
		// Move left as far as posible
		if (t->left != NULL) return findMin(t->left);
		// Min node found
		return t;
	}

	// Returns max node from specified subtree "t"
	node* findMax(node* t) const {
		if (t == NULL) return NULL;
		// Move right as far as posible
		if (t->right != NULL) return findMax(t->right);
		// Max node found
		return t;
	}

	// Retunrs subtree height
	int height(node* t) const {
		if (t == NULL) return -1;
		return t->height;
	}

	// Returns balance factor for specified node
	int getBalance(node* t) const {
		if (t == NULL) return 0;
		return height(t->left) - height(t->right);
	}

	void inorder(node* t) const {
		if (t == NULL) return;
		// Visit left subtree
		inorder(t->left);
		// Show subtree root
		cout << t->data << " ";
		// Visit right subtree
		inorder(t->right);
	}

	node* insert(const T& x, node* t) { //insert in AVL
		if (t == NULL) {
			t = new node();
			t->left = t->right = NULL;
			t->data = x;
			t->height = 0;
			++n;
			return t;
		}

		// Move left
		if (x < t->data) {
			t->left = insert(x, t->left);
			if (getBalance(t) == 2) {
				if (x < t->left->data)
					t = singleRightRotate(t);
				else
					t = doubleRightRotate(t);
			}
			t->height = max(height(t->left), height(t->right)) + 1;
			return t;
		}

		// Move right
		if (x > t->data) {
			t->right = insert(x, t->right);
			if (getBalance(t) == -2)
			{
				if (x > t->right->data)
					t = singleLeftRotate(t);
				else
					t = doubleLeftRotate(t);
			}
			t->height = max(height(t->left), height(t->right)) + 1;
			return t;
		}

		// No duplication allowed. do nothing 
		return t;
	}

	node* remove(int x, node* t) {
		if (t == NULL) return NULL;

		// Move left subtree
		if (x < t->data) t->left = remove(x, t->left);
		// Move right subtree
		else if (x > t->data) t->right = remove(x, t->right);
		// Found node with specified key
		else {

			node* temp = NULL;

			// Two childs case
			if (t->left != NULL && t->right != NULL) {
				temp = findMin(t->right);
				t->data = temp->data;
				t->right = remove(t->data, t->right);
			}
			else {
				temp = t;
				if (t->left == NULL) t = t->right;
				else if (t->right == NULL) t = t->left;

				--n;
				delete temp;
			}
		}

		if (t == NULL) return NULL;

		// Update height
		t->height = max(height(t->left), height(t->right)) + 1;

		// Balance subtree
		if (getBalance(t) == 2) {
			if (getBalance(t->left) == 1) return singleLeftRotate(t);
			return doubleLeftRotate(t);
		}

		if (getBalance(t) == -2) {
			if (getBalance(t->right) == -1) return singleRightRotate(t);
			return doubleRightRotate(t);
		}

		return t;
	}

	// Copy tree helper
	node* cloneTree(node* t) {
		if (t == NULL) return NULL;

		node* n = new node();
		n->data = t->data;
		n->height = t->height;

		n->left = cloneTree(t->left);
		n->right = cloneTree(t->right);

		return n;
	}

public:
	// Default ctor
	AVL()
		: root(NULL), n(0) { }

	// Copy ctor
	AVL(const AVL& tree)
		: root(NULL), n(0) {
		root = cloneTree(tree.root);
		n = tree.n;
	}

	// Destructor
	~AVL() { makeEmpty(root); }

	// Assignment operator
	AVL& operator=(const AVL& rhs) {
		if (this != &rhs) {
			AVL temp(rhs);
			swap(this->root, temp.root);
			swap(this->n, temp.n);
		}
		return *this;
	}

	void insert(int x) { root = insert(x, root); }

	void remove(int x) { root = remove(x, root); }

	void display() const { inorder(root); cout << endl; }

	int size() const { return n; }
private:
	node* root;
	int n;
};




class Rope {
	struct node_t;
	// Retunrs subtree height
	int height(node_t* t) const {
		if (t == nullptr) return -1;
		return t->h;
	}
private:
    struct node_t {
        string s;
		size_t w;
		int h;        
        node_t* left;
        node_t* right;

		static int height(node_t* t) {
			if (t == nullptr) return -1;
			return t->h;
		}

        node_t(const string& s, size_t weight)
            : s(s), w(weight), h(1), 
			left(nullptr), right(nullptr) { }

        node_t(node_t* left, node_t* right)
            : s(string()), w((left != nullptr ? left->w : 0) + (right != nullptr ? right->w : 0)), h(max(height(left), height(right)) + 1),
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

    void in_order_traverse_helper(node_t* node, function<void(const string&)> f) {
        if (node == nullptr) return;
        in_order_traverse_helper(node->left, f);
        f(node->s);
        in_order_traverse_helper(node->right, f);
    }

    void pre_order_traverse_helper(node_t* node, function<void(const string&)> f) {
        if (node == nullptr) return;
        f(node->s);
        pre_order_traverse_helper(node->left, f);
        pre_order_traverse_helper(node->right, f);
    }

    void helper_post_order_traverse(node_t* node, function<void(const string&)> f) {
        if (node == nullptr) return;
        helper_post_order_traverse(node->left, f);
        helper_post_order_traverse(node->right, f);
        f(node->s);
    }

	////////////////////
	node_t* singleLeftRotate(node_t*& t) {
		node_t* p = t->right;
		node_t* temp = p->left;

		/* Rotation */
		p->left = t;
		t->right = temp;

		/* Update heights */
		t->h = 1 + max(height(t->left), height(t->right));
		p->h = 1 + max(height(p->left), height(p->right));

		/* Return changed node */
		return p;
	}

	node_t* singleRightRotate(node_t*& t) {
		node_t* x = t->left;
		node_t* temp = x->right;

		/* Rotation */
		x->right = t;
		t->left = temp;

		/* Update height */
		t->h = 1 + max(height(t->left), height(t->right));
		x->h = 1 + max(height(x->left), height(x->right));

		/* Save changes */
		return x;
	}

	node_t* doubleLeftRotate(node_t*& t) {
		t->right = singleRightRotate(t->right);
		return singleLeftRotate(t);
	}

	node_t* doubleRightRotate(node_t*& t) {
		t->left = singleLeftRotate(t->left);
		return singleRightRotate(t);
	}


	// Returns balance factor for specified node
	int getBalance(node_t* t) const {
		if (t == nullptr) return 0;
		return height(t->left) - height(t->right);
	}

	pair<node_t*, node_t*> split(node_t* node, int i) {
		node_t* tree1 = nullptr, *tree2 = nullptr;
		if (node->left) {
			if (node->left->w >= i) {
				auto res = split(node->left, i);
				tree1 = res.first;
				tree2 = new node_t(res.second, node->right);
			} else {
				auto res = split(node->right, i - node->left->w);
				tree1 = new node_t(node->left, res.first);
				tree2 = res.second;
			}
		} else {
			tree1 = new node_t(node->s.substr(0, i), i);
			tree2 = new node_t(node->s.substr(i, node->s.size()), node->s.size() - i);
		}

		delete node;

		return make_pair(tree1, tree2);
	}

	node_t* merge(node_t* L, node_t* R) {
		return new node_t(L, R);
		//if (L == nullptr) return R;
		//if (R == nullptr) return L;
		//return new node_t(L, R);
	}

	node_t* insert(node_t* node, int insertIndex, string s) {
		auto it = split(node, insertIndex);
		node_t* tree1 = it.first, * tree3 = it.second;
		node_t* tree2 = new node_t(s, s.size());
		return merge(merge(tree1, tree2), tree3);
	}

    node_t* build_tree(const string& s) {
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
    /*Rope(const string& s)
        : root(nullptr) { root = build_tree(s); }*/

	Rope(const char* s)
		: root(nullptr) {
		root = build_tree(s);
	}

    ~Rope() { clear(); }
    
    bool empty() const { return root == nullptr; }
    void clear() { clear_helper(root); root = nullptr; }
    void in_order_traverse(function<void(const string&)> func) { in_order_traverse_helper(root, func); }
    void pre_order_traverse(function<void(const string&)> func) { pre_order_traverse_helper(root, func); }
    void post_order_traverse(function<void(const string&)> func) { helper_post_order_traverse(root, func); }

	char& operator[](size_t i) { return get(root, i); }
	
	void insert(int insertIndex, const string& s) {
		root = insert(root, insertIndex, s);
	}

    int h(node_t* node) {
        if (node == nullptr) return 0;

        return std::max(h(node->left), h(node->right)) + 1;
    }

    int max_height() {
        return h(root);
    }
	int max_height2() {
		return root->h;
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

	Rope tree(S);
	tree.in_order_traverse(f);
	while (q-- > 0) {
		cin >> i >> j >> k;
		tree.cut_and_insert(i, j, k);
		tree.in_order_traverse(f); cout << endl;
	}
    return 0;
}