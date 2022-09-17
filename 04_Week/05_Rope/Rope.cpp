#include <iostream>
#include <string>
#include <functional>
#include <algorithm>
#include <iomanip>
using namespace std;

#include <vld\vld.h>

class Rope {
private:
	struct node_t {
		string s;
		size_t w;
		node_t* left;
		node_t* right;

		inline node_t* reset(const string& s, size_t weight) {
			this->s = s;
			w = weight;
			left = right = nullptr;
			return this;
		}

		inline node_t* reset(node_t* left, node_t* right) {
			s = string();
			w = (left != nullptr ? left->w : 0) + (right != nullptr ? right->w : 0);
			this->left = left;
			this->right = right;
			return this;
		}

		node_t(const string& s, size_t weight) { reset(s, weight); }
		node_t(node_t* left, node_t* right) { reset(left, right); }
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

	void in_order_traverse_helper(node_t* node, function<void(const string&)> f) {
		if (node == nullptr) return;
		in_order_traverse_helper(node->left, f);
		f(node->s);
		in_order_traverse_helper(node->right, f);
	}

	pair<node_t*, node_t*> split(node_t* node, int i) {
		node_t* tree1 = nullptr, * tree2 = nullptr;
		if (node == nullptr) return make_pair(nullptr, nullptr);
		if (node->left) {
			if (node->left->w >= i) {
				auto res = split(node->left, i);
				tree1 = res.first;
				tree2 = new node_t(res.second, node->right);
				//tree2 = node->reset(res.second, node->right);
			}
			else {
				auto res = split(node->right, i - node->left->w);
				tree1 = new node_t(node->left, res.first);
				//tree1 = node->reset(node->left, res.first);
				tree2 = res.second;
			}
		}
		else {

			tree1 = new node_t(node->s.substr(0, i), i);
			tree2 = new node_t(node->s.substr(i, node->s.size()), node->s.size() - i);
			//tree1 = node->reset(node->s.substr(0, i), i);
		}

		delete node;
		return make_pair(tree1, tree2);
	}

	node_t* merge(node_t* L, node_t* R) {
		if (L == nullptr) return R;
		if (R == nullptr) return L;
		return new node_t(L, R);
	}

	node_t* insert(node_t* node, int insertIndex, const string& s) {
		auto it = split(node, insertIndex);
		node_t* tree1 = it.first, * tree3 = it.second;
		node_t* tree2 = new node_t(s, s.size());
		return merge(merge(tree1, tree2), tree3);
	}

	node_t* build_tree(const string& s) {
		return new node_t(s, s.size());
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
		: root(nullptr) {
		root = build_tree(s);
	}

	~Rope() { clear(); }

	void clear() { clear_helper(root); root = nullptr; }
	void in_order_traverse(function<void(const string&)> func) { in_order_traverse_helper(root, func); }
	char& operator[](size_t i) { return get(root, i); }

	void insert(int insertIndex, const string& s) {
		root = insert(root, insertIndex, s);
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
			/*auto R = split(root, j + 1);
			root = R.first;

			auto L = split(root, i);
			root = L.first;

			auto substr = L.second;
			root = merge(root, R.second);



			insert(k, substr->s);
			clear_helper(substr);*/

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
	tree.in_order_traverse(f); cout << endl;
	return 0;
}




















//////////////////////////////////////////////
//#include <iostream>
//#include <string>
//
//
//template <class T>
//class Rope {
//public:
//	explicit Rope(const std::string& s) {
//		root_ = new Node();
//		root_->s = new std::string(s);
//		root_->w = root_->s->length();
//	}
//
//	void insert(Rope* s, T pos) {
//		if (pos == 0) {
//			prepend(s);
//		}
//		else if (pos == root_->w) {
//			append(s);
//		}
//		else {
//			auto right = split(pos);
//			append(s);
//			append(right);
//		}
//	}
//
//	void prepend(Rope* b) {
//		auto node = b->getRoot();
//		std::swap(root_, node);
//		append(node);
//	}
//
//	void append(Rope* b) {
//		append(b->getRoot());
//	}
//
//	void print(std::ostream& out) {
//		print(root_, out);
//		out << std::endl;
//	}
//
//	Rope* split(T i) {
//		auto res = split(root_, i);
//		root_ = res.first;
//		return new Rope(res.second);
//	}
//
//private:
//	struct Node {
//		Node* left = nullptr;
//		Node* right = nullptr;
//		T w = 0;
//		std::string* s = nullptr;
//	};
//
//	explicit Rope(Node* node) {
//		root_ = node;
//	}
//
//	Node* getRoot() {
//		return root_;
//	}
//
//	std::pair<Node*, Node*> split(Node* tree1, T i) {
//		Node* tree2 = nullptr;
//		if (tree1->s == nullptr) {
//			if (tree1->left->w > i) {
//				auto res = split(tree1->left, i);
//				tree2 = new Node();
//				tree2->left = res.second;
//				tree2->right = tree1->right;
//				tree2->w = tree2->left->w + tree2->right->w;
//
//				delete tree1;
//				tree1 = res.first;
//			}
//			else {
//				auto res = split(tree1->right, i - tree1->left->w);
//				if (res.first->w == 0) {
//					delete res.first;
//					auto t = tree1->left;
//					delete tree1;
//					tree1 = t;
//				}
//				else {
//					tree1->right = res.first;
//					tree1->w = tree1->left->w + tree1->right->w;
//				}
//
//				tree2 = res.second;
//			}
//		}
//		else {
//			tree2 = new Node();
//			tree2->s = new std::string(tree1->s->substr(i));
//			tree2->w = tree2->s->length();
//
//			tree1->s->resize(i);
//			tree1->s->shrink_to_fit();
//			tree1->w = tree1->s->length();
//		}
//
//		return std::make_pair(tree1, tree2);
//	}
//
//	void append(Node* b) {
//		if (root_->w == 0) {
//			delete root_;
//			root_ = b;
//		}
//		else if (b->w > 0) {
//			auto newNode = new Node();
//			newNode->left = root_;
//			newNode->right = b;
//			newNode->w = newNode->left->w + newNode->right->w;
//			root_ = newNode;
//		}
//	}
//
//	void print(Node* node, std::ostream& out) {
//		if (node->s == nullptr) {
//			print(node->left, out);
//			print(node->right, out);
//		}
//		else {
//			out << *node->s;
//		}
//	}
//
//	Node* root_;
//};
//
//
//int main() {
//	auto s = std::string();
//	std::getline(std::cin, s);
//
//	int q;
//	std::cin >> q;
//
//	int i;
//	int j;
//	int k;
//
//	auto rope = Rope<int>(s);
//	while (q--) {
//		std::cin >> i >> j >> k;
//
//		auto right = rope.split(j + 1);
//		auto substr = rope.split(i);
//		rope.append(right);
//		rope.insert(substr, k);
//	}
//
//	rope.print(std::cout);
//
//	return 0;
//}