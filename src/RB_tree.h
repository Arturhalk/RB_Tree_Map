#include <iostream>
// ПРОТОТИП
// class RBTree {
//     struct Node {
//         Node(int value, Node* left = nullptr, Node* right = nullptr)
//             : value(value), left(left), right(right){};
//         int value;
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
//         if (root->value < a) {
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
//         if (tmp->value < a) {
//             while (tmp != nullptr) {
//                 if (tmp->right != nullptr) {
//                     if (tmp->right->value != a) {
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
//                     if (tmp->left->value != a) {
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
//         if (tmp->value > a) {
//             while (tmp != nullptr) {
//                 if (a != tmp->value) {
//                     tmp = tmp->left;
//                 } else {
//                     return true;
//                 }
//             }
//         } else {
//             while (tmp != nullptr) {
//                 if (a != tmp->value) {
//                     tmp = tmp->right;
//                 } else {
//                     return true;
//                 }
//             }
//         }
//         return false;
//     }

//     void print() {
//         Preorder(root);
//         std::cout << std::endl;
//     }

//    private:
//     void Preorder(Node* root) {
//         if (!root) {
//             return;
//         }
//         std::cout << root->value << " ";

//         Preorder(root->left);

//         Preorder(root->right);
//     }
// };

enum class Color { kBlack, kRed };
class RBTree {
    struct Node {
        Node(int val_, Node* parent_ = nullptr, Node* left_ = nullptr,
             Node* right_ = nullptr)
            : value(val_), left(left_), right(right_), parent(parent_) {};
        int value;
        Node* left;
        Node* right;
        Color color = Color::kRed;
        Node* parent;
    };
    Node* root = nullptr;
    void Preorder(Node* root) {
        if (!root) {
            return;
        }
        std::cout << root->value << " ";

        Preorder(root->left);

        Preorder(root->right);
    }

    bool IsUnbalanceRightRight(Node* current) {
        Node* grand = current->parent->parent;
        return current->parent == grand->right &&
               current->parent->color == Color::kRed &&
               current == current->parent->right;
    }
    bool IsUnbalanceLeftLeft(Node* current) {
        Node* grand = current->parent->parent;
        return current->parent == grand->left &&
               current->parent->color == Color::kRed &&
               current == current->parent->left;
    }
    bool IsUnbalanceRightLeft(Node* current) {
        Node* grand = current->parent->parent;
        return current->parent == grand->right &&
               current->parent->color == Color::kRed &&
               current == current->parent->left;
    }
    bool IsUnbalanceLeftRight(Node* current) {
        Node* grand = current->parent->parent;
        return current->parent == grand->left &&
               current->parent->color == Color::kRed &&
               current == current->parent->right;
    }
    void RotateRightRight(Node* tmp) {
        Node* grand = tmp->parent->parent;
        Node* brother = tmp->parent->left;
        // Перестраиваем дерево
        if (grand->parent && grand->parent->right == grand) {
            grand->parent->right = tmp->parent;
            grand->right = nullptr;
        } else if (grand->parent && grand->parent->left == grand) {
            grand->parent->left = tmp->parent;
        }
        tmp->parent->left = grand;
        tmp->parent->left->right = brother;
        // Меняем зависимость узлов(родителей и предков)
        tmp->parent->parent = grand->parent;
        tmp->parent->left->parent = tmp->parent;
        if (brother) {
            brother->parent = grand;
        }
        if (grand == root) {
            root = tmp->parent;
        }
        // Перекрашеваем
        tmp->parent->left->color = Color::kRed;
        tmp->parent->color = Color::kBlack;
    }
    void RotateLeftLeft(Node* tmp) {
        Node* grand = tmp->parent->parent;
        Node* brother = tmp->parent->right;
        // Перестраиваем дерево
        if (grand->parent) {
            grand->parent->left = tmp->parent;
            grand->left = nullptr;
        } else if (grand->parent && grand->parent->left == grand) {
            grand->parent->left = tmp->parent;
        }
        tmp->parent->right = grand;
        tmp->parent->right->left = brother;
        // Меняем зависимость узлов(родителей и предков)
        tmp->parent->parent = grand->parent;
        tmp->parent->right->parent = tmp->parent;
        if (brother) {
            brother->parent = grand;
        }
        if (grand == root) {
            root = tmp->parent;
        }
        // Перекрашеваем
        tmp->parent->right->color = Color::kRed;
        tmp->parent->color = Color::kBlack;
    }
    void RotateRightLeft(Node* tmp) {
        Node* grand = tmp->parent->parent;
        Node* parent = tmp->parent;
        Node* right_current = tmp->right;
        // Меняем местами parent и current
        grand->right = tmp;
        tmp->right = parent;
        // Меняем зависимости снова)
        tmp->parent = grand;
        parent->parent = tmp;
        parent->left = right_current;
        // Переходим к разбалансировке RR
        RotateRightRight(tmp->right);
    }
    void RotateLeftRight(Node* tmp) {
        Node* grand = tmp->parent->parent;  // 12
        Node* parent = tmp->parent;         // 3
        Node* left_current = tmp->left;
        // Меняем местами parent и current
        grand->left = tmp;
        tmp->left = parent;
        // Меняем зависимости снова)
        tmp->parent = grand;
        parent->parent = tmp;
        parent->right = left_current;
        // Переходим к разбалансировке RR
        RotateLeftLeft(tmp->left);
    }
    void Rotate(Node* tmp) {
        Node* grand = tmp->parent->parent;
        if (!grand) {
            return;
        }
        if (IsUnbalanceRightRight(tmp)) {
            RotateRightRight(tmp);
        } else if (IsUnbalanceLeftLeft(tmp)) {
            RotateLeftLeft(tmp);
        } else if (IsUnbalanceRightLeft(tmp)) {
            RotateRightLeft(tmp);
        } else if (IsUnbalanceLeftRight(tmp)) {
            RotateLeftRight(tmp);
        }
    }
    Node* SubInsert(int value) {
        Node* tmp = root;
        while (tmp) {
            if (tmp->value > value) {
                if (!tmp->left) {
                    tmp->left = new Node(value, tmp);
                    tmp = tmp->left;
                    break;
                }
                tmp = tmp->left;
            } else {
                if (!tmp->right) {
                    tmp->right = new Node(value, tmp);
                    tmp = tmp->right;
                    break;
                }
                tmp = tmp->right;
            }
        }
        return tmp;
    }
    bool PrepairEraseLeftRight(Node* tmp) {
        Node* left_curr = tmp->left;
        Node* right_curr = tmp->right;
        Node* parent_curr = tmp->parent;
        if (left_curr && left_curr->right) {
            Node* lr = left_curr->right;
            while (lr->right) {
                lr = lr->right;
            }
            if (tmp->parent && isLeftSon(tmp)) {
                tmp->parent->left = lr;
            } else if (tmp->parent && isRightSon(tmp)) {
                tmp->parent->right = lr;
            }
            Node* lr_child_left = lr->left;
            Node* lr_parent = lr->parent;
            lr_parent->right = tmp;

            // полностью переместили удаляемый элемент
            tmp->left = lr_child_left;
            tmp->right = nullptr;
            // переместим вместо удаляемого
            lr->left = left_curr;
            lr->right = right_curr;
            lr->parent = tmp->parent;
            // сменим родителя у удаляемого
            tmp->parent = lr_parent;
            // меняем цвета
            // Color color_tmp = tmp->color;
            // tmp->color = lr->color;
            // lr->color = color_tmp;
            // сохраняем новое дерево
            if (root == tmp) {
                root = &(*lr);
            }
            return true;
        }
        return false;
    }
    bool PrepairEraseRightLeft(Node* tmp) {
        Node* left_curr = tmp->left;
        Node* right_curr = tmp->right;
        Node* parent_curr = tmp->parent;
        if (right_curr && right_curr->left) {
            Node* rl = right_curr->left;
            while (rl->left) {
                rl = rl->left;
            }
            if (tmp->parent && isLeftSon(tmp)) {
                tmp->parent->left = rl;
            } else if (tmp->parent && isRightSon(tmp)) {
                tmp->parent->right = rl;
            }
            Node* rl_child_right = rl->right;
            Node* rl_parent = rl->parent;
            rl->parent->left = tmp;
            // Все аналогично лево-правому удалению
            tmp->right = rl_child_right;
            tmp->left = nullptr;

            rl->left = left_curr;
            rl->right = right_curr;
            rl->parent = tmp->parent;

            tmp->parent = rl_parent;

            // Color color_tmp = tmp->color;
            // tmp->color = rl->color;
            // rl->color = color_tmp;

            root = &(*rl);
            return true;
        }
        return false;
    }
    bool EraseWithOneChild(Node*& tmp) {
        if (!tmp->left && tmp->right) {
            tmp->right->parent = tmp->parent;
            tmp->parent->right = tmp->right;
            delete tmp;
            tmp = nullptr;
            return true;
        }
        if (tmp->left && !tmp->right) {
            tmp->left->parent = tmp->parent;
            tmp->parent->left = tmp->left;
            delete tmp;
            tmp = nullptr;
            return true;
        }
        return false;
    }
    bool isLeftSon(Node* tmp) { return tmp == tmp->parent->left; }
    bool isRightSon(Node* tmp) { return tmp == tmp->parent->right; }
    bool EraseWithoutChilds(Node* tmp) {
        if (!tmp->left && !tmp->right) {
            if (tmp == tmp->parent->right) {
                tmp->parent->right = nullptr;
            } else if (tmp == tmp->parent->left) {
                tmp->parent->left = nullptr;
            }
            delete tmp;
            return true;
        }
        return false;
    }
    bool EraseWithTwoChilds(Node*& tmp) {
        if (PrepairEraseLeftRight(tmp)) {
            if (EraseWithOneChild(tmp)) {
                return true;
            } else {
                EraseWithoutChilds(tmp);
            }
            return true;
        }
        if (PrepairEraseRightLeft(tmp)) {
            if (EraseWithOneChild(tmp)) {
                return true;
            } else {
                EraseWithoutChilds(tmp);
            }
            return true;
        }
        return false;
    }

    // void PlaceToDeletedNode(Node* tmp) {
    //     if(){

    //     }
    // }
    void SubErase(int value) {
        Node* tmp = root;
        while (tmp) {
            if (tmp->value > value) {
                tmp = tmp->left;
            } else if (tmp->value < value) {
                tmp = tmp->right;
            } else if (tmp->value == value) {
                if (EraseWithoutChilds(tmp)) {
                    return;
                }
                if (EraseWithOneChild(tmp)) {
                    return;
                }
                if (EraseWithTwoChilds(tmp)) {
                    return;
                }
            }
        }
    }
    void Repaint(Node* current) {
        Node* grand = current->parent->parent;
        if (!grand) {
            return;
        }

        if (current->parent == grand->right && grand->left &&
            grand->right->color == Color::kRed &&
            grand->left->color == Color::kRed) {
            grand->left->color = Color::kBlack;
            current->parent->color = Color::kBlack;
        } else if (current->parent == grand->left && grand->right &&
                   grand->right->color == Color::kRed &&
                   grand->left->color == Color::kRed) {
            grand->right->color = Color::kBlack;
            current->parent->color = Color::kBlack;
        }
        if (grand != root) {
            grand->color = Color::kRed;
        }
    }

   public:
    void Insert(int value) {
        if (!root) {  // Если дерева не существует
            root = new Node(value, root);
            root->color = Color::kBlack;
            return;
        }
        Node* insert_node = SubInsert(value);
        while (insert_node && insert_node != root) {
            Repaint(insert_node);
            Rotate(insert_node);
            if (!insert_node->parent) {
                break;
            }
            insert_node = insert_node->parent->parent;
        }
    }

    void Print() {
        Preorder(root);
        std::cout << std::endl;
    }

    void Erase(int value) {
        if (!root) {
            return;
        }
        SubErase(value);
    }
    bool Find(int value) {
        Node* current = root;
        if (!root) {
            return false;
        }
        while (current != nullptr) {
            if (value < current->value) {
                if (current->left != nullptr) {
                    current = current->left;
                } else {
                    return false;
                }
            }
            if (value > current->value) {
                if (current->right != nullptr) {
                    current = current->right;
                } else {
                    return false;
                }
            }
            if (value == current->value) {
                return true;
            }
        }
        return false;
    }
};
