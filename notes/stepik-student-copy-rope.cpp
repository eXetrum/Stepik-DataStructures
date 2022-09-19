////////////////////////////////////////////
#include <iostream>
#include <string>


template <class T>
class Rope {
public:
	explicit Rope(const std::string& s) {
		root_ = new Node();
		root_->s = new std::string(s);
		root_->w = root_->s->length();
	}

	void insert(Rope* s, T pos) {
		if (pos == 0) {
			prepend(s);
		}
		else if (pos == root_->w) {
			append(s);
		}
		else {
			auto right = split(pos);
			append(s);
			append(right);
		}
	}

	void prepend(Rope* b) {
		auto node = b->getRoot();
		std::swap(root_, node);
		append(node);
	}

	void append(Rope* b) {
		append(b->getRoot());
	}

	void print(std::ostream& out) {
		print(root_, out);
		out << std::endl;
	}

	Rope* split(T i) {
		auto res = split(root_, i);
		root_ = res.first;
		return new Rope(res.second);
	}

private:
	struct Node {
		Node* left = nullptr;
		Node* right = nullptr;
		T w = 0;
		std::string* s = nullptr;
	};

	explicit Rope(Node* node) {
		root_ = node;
	}

	Node* getRoot() {
		return root_;
	}

	std::pair<Node*, Node*> split(Node* tree1, T i) {
		Node* tree2 = nullptr;
		if (tree1->s == nullptr) {
			if (tree1->left->w > i) {
				auto res = split(tree1->left, i);
				tree2 = new Node();
				tree2->left = res.second;
				tree2->right = tree1->right;
				tree2->w = tree2->left->w + tree2->right->w;

				delete tree1;
				tree1 = res.first;
			}
			else {
				auto res = split(tree1->right, i - tree1->left->w);
				if (res.first->w == 0) {
					delete res.first;
					auto t = tree1->left;
					delete tree1;
					tree1 = t;
				}
				else {
					tree1->right = res.first;
					tree1->w = tree1->left->w + tree1->right->w;
				}

				tree2 = res.second;
			}
		}
		else {
			tree2 = new Node();
			tree2->s = new std::string(tree1->s->substr(i));
			tree2->w = tree2->s->length();

			tree1->s->resize(i);
			tree1->s->shrink_to_fit();
			tree1->w = tree1->s->length();
		}

		return std::make_pair(tree1, tree2);
	}

	void append(Node* b) {
		if (root_->w == 0) {
			delete root_;
			root_ = b;
		}
		else if (b->w > 0) {
			auto newNode = new Node();
			newNode->left = root_;
			newNode->right = b;
			newNode->w = newNode->left->w + newNode->right->w;
			root_ = newNode;
		}
	}

	void print(Node* node, std::ostream& out) {
		if (node->s == nullptr) {
			print(node->left, out);
			print(node->right, out);
		}
		else {
			out << *node->s;
		}
	}

	Node* root_;
};


int main() {
	auto s = std::string();
	std::getline(std::cin, s);

	int q;
	std::cin >> q;

	int i;
	int j;
	int k;

	auto rope = Rope<int>(s);
	while (q--) {
		std::cin >> i >> j >> k;

		auto right = rope.split(j + 1);
		auto substr = rope.split(i);
		rope.append(right);
		rope.insert(substr, k);
	}

	rope.print(std::cout);

	return 0;
}