#include <iostream>
#include <set>
class RBTree {
	struct Node {
		Node(int val, Node* left = nullptr, Node* right = nullptr) : val(val), left(left), right(right) {};
		// int height;
		// std::string color = "red";
		// int key;
		int val;
		Node* left;
		Node* right;
	};
	Node* root = nullptr;
public:
	void insert(int a) {
		if (!root) {
        	root = new Node(a);
        	return;
    	}
		Node* tmp = root;
		if (root->val < a) {
			while(tmp->right != nullptr){
				tmp = tmp->right;
			}
			tmp->right = new Node(a);
		} else {
			while(tmp->left != nullptr) {
				tmp = tmp->left;
			}
			tmp->left = new Node(a);
		}
	}

	void erase(int a) {
		if(!root) {
			return;
		}
	}

	bool find(int a) {
		if(!root){
			return false;
		}
		Node* tmp = root;
		if(tmp->val > a) {
			while(tmp != nullptr) {
				if(a != tmp->val){
					tmp = tmp->left;
				} else {
					return true;
				}
			}
		} else {
			while(tmp != nullptr) {
				if(a != tmp->val){
					tmp = tmp->right;
				} else {
					return true;
				}
			}
		}
		return false;
	}
};
