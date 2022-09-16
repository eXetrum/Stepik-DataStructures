#include <iostream>
#include <string>
#include <functional>
using namespace std;


#include <iostream>
#include <algorithm>
using namespace std;

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




template <class Type>
class Rope {
    struct node_t {
        Type key;
        size_t w;
        node_t* left;
        node_t* right;

        node_t(const Type& data, size_t weight)
            : key(data), w(weight), left(nullptr), right(nullptr) { }

        node_t(node_t* left, node_t* right)
            : key(Type()), w((left != nullptr ? left->w : 0) + (right != nullptr ? right->w : 0)),
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

    node_t* merge(node_t* n1, node_t* n2) {
        return new node_t(n1, n2);
    }

    node_t* build_tree(const string& s) {
        node_t* cur = nullptr;
        for (size_t i = 0; i < s.size(); ++i) {
            cur = merge(cur, new node_t(s[i], 1));
        }
        return cur;
    }
public:
    Rope(const string& s) 
        : root(nullptr) { root = build_tree(s); }

    ~Rope() { clear(); }
    
    bool empty() const { return root == nullptr; }
    void clear() { clear_helper(root); root = nullptr; }
    void in_order_traverse(function<void(const Type& key)> func) { in_order_traverse_helper(root, func); }
    void pre_order_traverse(function<void(const Type& key)> func) { pre_order_traverse_helper(root, func); }
    void post_order_traverse(function<void(const Type& key)> func) { helper_post_order_traverse(root, func); }

    int h(node_t* node) {
        if (node == nullptr) return 0;

        return std::max(h(node->left) + 1, h(node->right) + 1);
    }

    int max_height() {
        return h(root);
    }
};

int main() {
    string S;
    int q, i, j, k;
    function<void(int)> f = [](const char& k) {
        cout << k << " ";
    };

    cin >> S >> q;
    Rope<char> rope(S);
    rope.in_order_traverse(f);
    cout << endl << "h=" << rope.max_height() << endl;


    while(q-- > 0) {
        cin >> i >> j >> k;
    }

    return 0;
}