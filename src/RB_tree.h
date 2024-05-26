#include <iostream>
#include <set>
// ПРОТОТИП
// class RBTree {
//     struct Node {
//         Node(int val, Node* left = nullptr, Node* right = nullptr)
//             : val(val), left(left), right(right){};
//         int val;
//         Node* left;
//         Node* right;
//     };
//     Node* root = nullptr;

//    public:
//     void insert(int a) {
//         if (!root) {
//             root = new Node(a);
//             return;
//         }
//         Node* tmp = root;
//         if (root->val < a) {
//             while (tmp->right != nullptr) {
//                 tmp = tmp->right;
//             }
//             tmp->right = new Node(a);
//         } else {
//             while (tmp->left != nullptr) {
//                 tmp = tmp->left;
//             }
//             tmp->left = new Node(a);
//         }
//     }

//     void erase(int a) {
//         if (!root) {
//             return;
//         }
//         Node* tmp = root;
//         if (tmp->val < a) {
//             while (tmp != nullptr) {
//                 if (tmp->right != nullptr) {
//                     if (tmp->right->val != a) {
//                         tmp = tmp->right;
//                     } else {
//                         Node* remem = tmp->right->right;
//                         delete tmp->right;
//                         tmp->right = remem;
//                     }
//                 } else {
//                     return;
//                 }
//             }
//         } else {
//             while (tmp != nullptr) {
//                 if (tmp->left != nullptr) {
//                     if (tmp->left->val != a) {
//                         tmp = tmp->left;
//                     } else {
//                         Node* remem = tmp->left->left;
//                         delete tmp->left;
//                         tmp->left = remem;
//                     }
//                 } else {
//                     return;
//                 }
//             }
//         }
//     }

//     bool find(int a) {
//         if (!root) {
//             return false;
//         }
//         Node* tmp = root;
//         if (tmp->val > a) {
//             while (tmp != nullptr) {
//                 if (a != tmp->val) {
//                     tmp = tmp->left;
//                 } else {
//                     return true;
//                 }
//             }
//         } else {
//             while (tmp != nullptr) {
//                 if (a != tmp->val) {
//                     tmp = tmp->right;
//                 } else {
//                     return true;
//                 }
//             }
//         }
//         return false;
//     }

//     void print() {
//         preorder(root);
//         std::cout << std::endl;
//     }

//    private:
//     void preorder(Node* root) {
//         if (!root) {
//             return;
//         }
//         std::cout << root->val << " ";

//         preorder(root->left);

//         preorder(root->right);
//     }
// };

enum class Color { black, red };
class RBtree {
    struct Node {
        Node(int val_, Node* parent_ = nullptr, Node* left_ = nullptr,
             Node* right_ = nullptr)
            : val(val_), left(left_), right(right_), parent(parent_){};
        int val;
        Node* left;
        Node* right;
        Color color = Color::red;
        Node* parent;
    };
    Node* header = nullptr;
    void preorder(Node* root) {
        if (!root) {
            return;
        }
        std::cout << root->val << " ";

        preorder(root->left);

        preorder(root->right);
    }
    bool ifUnbalanceRR(Node* current) {
        Node* grand = current->parent->parent;
        return current->parent == grand->right &&
               current->parent->color == Color::red &&
               current == current->parent->right;
    }
    bool ifUnbalanceLL(Node* current) {
        Node* grand = current->parent->parent;
        return current->parent == grand->left &&
               current->parent->color == Color::red &&
               current == current->parent->left;
    }
    bool ifUnbalanceRL(Node* current) {
        Node* grand = current->parent->parent;
        return current->parent == grand->right &&
               current->parent->color == Color::red &&
               current == current->parent->left;
    }
    bool ifUnbalanceLR(Node* current) {
        Node* grand = current->parent->parent;
        return current->parent == grand->left &&
               current->parent->color == Color::red &&
               current == current->parent->right;
    }
    void rotateRR(Node* tmp) {
        Node* grand = tmp->parent->parent;
        Node* brother = tmp->parent->left;
        // Перестраиваем дерево
        tmp->parent->left = grand;
        tmp->parent->left->right = brother;
        // Меняем зависимость узлов(родителей и предков)
        tmp->parent->parent = grand->parent;
        tmp->parent->left->parent = tmp->parent;
        if (brother) {
            brother->parent = grand;
        }
        if (grand == header->right) {
            header->right = tmp->parent;
        }
        // Перекрашеваем
        tmp->parent->left->color = Color::red;
        tmp->parent->color = Color::black;
    }
    void rotateLL(Node* tmp) {
        Node* grand = tmp->parent->parent;
        Node* brother = tmp->parent->right;
        // Перестраиваем дерево
        tmp->parent->right = grand;
        tmp->parent->right->left = brother;
        // Меняем зависимость узлов(родителей и предков)
        tmp->parent->parent = grand->parent;
        tmp->parent->right->parent = tmp->parent;
        if (brother) {
            brother->parent = grand;
        }
        if (grand == header->right) {
            header->right = tmp->parent;
        }
        // Перекрашеваем
        tmp->parent->right->color = Color::red;
        tmp->parent->color = Color::black;
    }
    void isUnbalanceRL(Node* tmp) {
        Node* grand = tmp->parent->parent;
        Node* remParent = tmp->parent;
        // Меняем местами parent и current
        grand->right = tmp;
        tmp->right = remParent;
        //  Меняем зависимости снова)
        tmp = tmp->right;
        tmp->parent = grand->right;
        tmp->parent->parent = grand;
        remParent->left = nullptr;
        // Переходим к разбалансировке RR
        rotateRR(tmp);
    }
    void isUnbalanceLR(Node* tmp) {
        Node* grand = tmp->parent->parent;
        Node* remParent = tmp->parent;
        // Меняем местами parent и current
        grand->left = tmp;
        tmp->left = remParent;
        // Меняем зависимости снова)
        tmp = tmp->left;
        tmp->parent = grand->left;
        tmp->parent->parent = grand;
        remParent->right = nullptr;
        // Переходим к разбалансировке RR
        rotateLL(tmp);
    }
    void Rotate(Node* tmp) {
        Node* grand = tmp->parent->parent;
        if (!grand || grand == header) {
            return;
        }
        if (ifUnbalanceRR(tmp)) {
            rotateRR(tmp);
        } else if (ifUnbalanceLL(tmp)) {
            rotateLL(tmp);
        } else if (ifUnbalanceRL(tmp)) {
            isUnbalanceRL(tmp);
        } else if (ifUnbalanceLR(tmp)) {
            isUnbalanceLR(tmp);
        }
    }
    Node* SubInsert(int val) {
        Node* tmp = header->right;
        while (tmp) {
            if (tmp->val > val) {
                if (!tmp->left) {
                    tmp->left = new Node(val, tmp);
                    tmp = tmp->left;
                    break;
                }
                tmp = tmp->left;
            } else {
                if (!tmp->right) {
                    tmp->right = new Node(val, tmp);
                    tmp = tmp->right;
                    break;
                }
                tmp = tmp->right;
            }
        }
        return tmp;
    }
    void Repaint(Node* current) {
        Node* grand = current->parent->parent;
        if (!grand) {
            return;
        }
        if (grand != header->right) {
            grand->color = Color::red;
        }
        if (current->parent == grand->right && grand->left) {
            grand->left->color = Color::black;
        } else if (current->parent == grand->left && grand->right) {
            grand->right->color = Color::black;
        }
    }

   public:
    RBtree() { header = new Node(INT_MIN); }
    void insert(int value) {
        if (!header->right) {  // Если дерева не существует
            header->right = new Node(value, header);
            header->right->color = Color::black;
            return;
        }
        Node* insertedNode = SubInsert(value);
        Repaint(insertedNode);
        Rotate(insertedNode);
    }

    void print() {
        preorder(header->right);
        std::cout << std::endl;
    }

    void erase(int value) {}
    bool find(int value) { return false; }
};
