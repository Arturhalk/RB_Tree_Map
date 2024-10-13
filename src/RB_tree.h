#include <iostream>
#include <vector>
enum class Color
{
    kBlack,
    kRed
};
struct InfoTree
{
    Color color;
    int value;
    bool operator==(InfoTree const &a) const
    {
        if (color == a.color && value == a.value)
        {
            return true;
        }
        return false;
    }
};
class RBTree
{
    struct Node
    {
        Node(int val_, Node *parent_ = nullptr, Node *left_ = nullptr,
             Node *right_ = nullptr)
            : value(val_), left(left_), right(right_), parent(parent_) {};
        int value;
        Node *left;
        Node *right;
        Color color = Color::kRed;
        Node *parent;
    };
    Node *root = nullptr;
    void Preorder(Node *root, std::vector<InfoTree> &info)
    {
        if (!root)
        {
            return;
        }
        InfoTree a;
        a.color = root->color;
        a.value = root->value;
        info.push_back(a);

        Preorder(root->left, info);
        Preorder(root->right, info);
    }

    bool IsUnbalanceRightRight(Node *current)
    {
        Node *grand = current->parent->parent;
        return current->parent == grand->right &&
               current->parent->color == Color::kRed &&
               current == current->parent->right;
    }
    bool IsUnbalanceLeftLeft(Node *current)
    {
        Node *grand = current->parent->parent;
        return current->parent == grand->left &&
               current->parent->color == Color::kRed &&
               current == current->parent->left;
    }
    bool IsUnbalanceRightLeft(Node *current)
    {
        Node *grand = current->parent->parent;
        return current->parent == grand->right &&
               current->parent->color == Color::kRed &&
               current == current->parent->left;
    }
    bool IsUnbalanceLeftRight(Node *current)
    {
        Node *grand = current->parent->parent;
        return current->parent == grand->left &&
               current->parent->color == Color::kRed &&
               current == current->parent->right;
    }
    void RotateRightRight(Node *tmp)
    {
        Node *grand = tmp->parent->parent;
        Node *brother = tmp->parent->left;
        // Перестраиваем дерево
        if (grand->parent && grand->parent->right == grand)
        {
            grand->parent->right = tmp->parent;
            grand->right = nullptr;
        }
        else if (grand->parent && grand->parent->left == grand)
        {
            grand->parent->left = tmp->parent;
        }
        tmp->parent->left = grand;
        tmp->parent->left->right = brother;
        // Меняем зависимость узлов(родителей и предков)
        tmp->parent->parent = grand->parent;
        tmp->parent->left->parent = tmp->parent;
        if (brother)
        {
            brother->parent = grand;
        }
        if (grand == root)
        {
            root = tmp->parent;
        }
        // Перекрашеваем
        tmp->parent->left->color = Color::kRed;
        tmp->parent->color = Color::kBlack;
    }
    void RotateLeftLeft(Node *tmp)
    {
        Node *grand = tmp->parent->parent;
        Node *brother = tmp->parent->right;
        // Перестраиваем дерево
        if (grand->parent && grand->parent->right == grand)
        {
            grand->parent->right = tmp->parent;
            grand->right = nullptr;
        }
        else if (grand->parent && grand->parent->left == grand)
        {
            grand->parent->left = tmp->parent;
        }
        tmp->parent->right = grand;
        tmp->parent->right->left = brother;
        // Меняем зависимость узлов(родителей и предков)
        tmp->parent->parent = grand->parent;
        tmp->parent->right->parent = tmp->parent;
        if (brother)
        {
            brother->parent = grand;
        }
        if (grand == root)
        {
            root = tmp->parent;
        }
        // Перекрашеваем
        tmp->parent->right->color = Color::kRed;
        tmp->parent->color = Color::kBlack;
    }
    void RotateRightLeft(Node *tmp)
    {
        Node *grand = tmp->parent->parent;
        Node *parent = tmp->parent;
        Node *right_current = tmp->right;
        // Меняем местами parent и current
        grand->right = tmp;
        tmp->right = parent;
        // Меняем зависимости
        tmp->parent = grand;
        parent->parent = tmp;
        parent->left = right_current;
        // Переходим к разбалансировке RR
        RotateRightRight(tmp->right);
    }
    void RotateLeftRight(Node *tmp)
    {
        Node *grand = tmp->parent->parent;
        Node *parent = tmp->parent;
        Node *left_current = tmp->left;
        // Меняем местами parent и current
        grand->left = tmp;
        tmp->left = parent;
        // Меняем зависимости
        tmp->parent = grand;
        parent->parent = tmp;
        parent->right = left_current;
        // Переходим к разбалансировке RR
        RotateLeftLeft(tmp->left);
    }
    void InsertRotate(Node *tmp)
    {
        Node *grand = tmp->parent->parent;
        if (!grand)
        {
            return;
        }
        if (IsUnbalanceRightRight(tmp))
        {
            RotateRightRight(tmp);
        }
        else if (IsUnbalanceLeftLeft(tmp))
        {
            RotateLeftLeft(tmp);
        }
        else if (IsUnbalanceRightLeft(tmp))
        {
            RotateRightLeft(tmp);
        }
        else if (IsUnbalanceLeftRight(tmp))
        {
            RotateLeftRight(tmp);
        }
    }
    Node *SubInsert(int value)
    {
        Node *tmp = root;
        while (tmp)
        {
            if (tmp->value > value)
            {
                if (!tmp->left)
                {
                    tmp->left = new Node(value, tmp);
                    tmp = tmp->left;
                    break;
                }
                tmp = tmp->left;
            }
            else
            {
                if (!tmp->right)
                {
                    tmp->right = new Node(value, tmp);
                    tmp = tmp->right;
                    break;
                }
                tmp = tmp->right;
            }
        }
        return tmp;
    }
    bool PrepairEraseLeftRight(Node *tmp)
    {
        // Приводим удаление с двумя детьми, к удалению с одним или без детей
        Node *left_curr = tmp->left;
        Node *right_curr = tmp->right;
        Node *parent_curr = tmp->parent;
        if (left_curr && left_curr->right)
        {
            Node *lr = left_curr->right;
            while (lr->right)
            {
                lr = lr->right;
            }
            if (tmp->parent && isLeftSon(tmp))
            {
                tmp->parent->left = lr;
            }
            else if (tmp->parent && isRightSon(tmp))
            {
                tmp->parent->right = lr;
            }
            Node *lr_child_left = lr->left;
            Node *lr_parent = lr->parent;
            lr_parent->right = tmp;

            // полностью переместили удаляемый элемент
            tmp->left = lr_child_left;
            tmp->right = nullptr;
            // переместим вместо удаляемого
            lr->left = left_curr;
            lr->right = right_curr;
            lr->parent = tmp->parent;
            lr_parent->parent = lr;
            // сменим родителя у удаляемого
            tmp->parent = lr_parent;
            // сменим родителей у заменяемого
            lr->left->parent = lr;
            lr->right->parent = lr;
            // меняем цвета
            Color color_tmp = tmp->color;
            tmp->color = lr->color;
            lr->color = color_tmp;
            // сохраняем новое дерево
            if (root == tmp)
            {
                root = &(*lr); // Применяем изменения к дереву
            }
            return true;
        }
        else if (left_curr && !left_curr->right)
        {
            if (tmp->parent && isLeftSon(tmp))
            {
                tmp->parent->left = left_curr;
            }
            else if (tmp->parent && isRightSon(tmp))
            {
                tmp->parent->right = left_curr;
            }
            Node *left_left_son = left_curr->left;
            // меняем местоположение узла справа
            left_curr->left = tmp;
            left_curr->right = tmp->right;
            // меняем местоположение узла tmp
            tmp->left = left_left_son;
            tmp->right = nullptr;
            // меняем родителей
            left_curr->right->parent = left_curr;
            left_curr->parent = tmp->parent;
            tmp->parent = left_curr;
            if (left_left_son)
            {
                left_left_son->parent = left_curr->left;
            }
            // меняем цвета
            Color color_tmp = tmp->color;
            tmp->color = left_curr->color;
            left_curr->color = color_tmp;
            return true;
        }
        return false;
    }
    bool PrepairEraseRightLeft(Node *tmp)
    {
        // Приводим удаление с двумя детьми, к удалению с одним или без детей
        Node *left_curr = tmp->left;
        Node *right_curr = tmp->right;
        Node *parent_curr = tmp->parent;
        if (right_curr && right_curr->left)
        {
            Node *rl = right_curr->left;
            while (rl->left)
            {
                rl = rl->left;
            }
            if (tmp->parent && isLeftSon(tmp))
            {
                tmp->parent->left = rl;
            }
            else if (tmp->parent && isRightSon(tmp))
            {
                tmp->parent->right = rl;
            }
            Node *rl_child_right = rl->right;
            Node *rl_parent = rl->parent;
            rl->parent->left = tmp;
            // Все аналогично лево-правому удалению
            tmp->right = rl_child_right;
            tmp->left = nullptr;

            rl->left = left_curr;
            rl->right = right_curr;
            rl->parent = tmp->parent;
            rl_parent->parent = rl;

            tmp->parent = rl_parent;

            rl->left->parent = rl;
            rl->right->parent = rl;

            Color color_tmp = tmp->color;
            tmp->color = rl->color;
            rl->color = color_tmp;
            if (root == tmp)
            {
                root = &(*rl); // Применяем изменения к дереву
            }
            return true;
        }
        else if (right_curr && !right_curr->left)
        {
            if (tmp->parent && isLeftSon(tmp))
            {
                tmp->parent->left = right_curr;
            }
            else if (tmp->parent && isRightSon(tmp))
            {
                tmp->parent->right = right_curr;
            }
            Node *right_right_son = right_curr->right;
            // меняем местоположение узла справа
            right_curr->right = tmp;
            right_curr->left = tmp->left;
            // меняем местоположение узла tmp
            tmp->right = right_right_son;
            tmp->left = nullptr;
            // меняем родителей
            right_curr->left->parent = right_curr;
            right_curr->parent = tmp->parent;
            tmp->parent = right_curr;
            if (right_right_son)
            {
                right_right_son->parent = right_curr->right;
            }
            // меняем цвета
            Color color_tmp = tmp->color;
            tmp->color = right_curr->color;
            right_curr->color = color_tmp;
            return true;
        }
        return false;
    }
    bool EraseWithOneChild(Node *&tmp)
    {
        if (!tmp)
        {
            return false;
        }
        if (!tmp->left && tmp->right)
        {
            tmp->right->parent = tmp->parent;
            tmp->parent->right = tmp->right;
            delete tmp;
            tmp = nullptr;
            return true;
        }
        if (tmp->left && !tmp->right)
        {
            tmp->left->parent = tmp->parent;
            tmp->parent->left = tmp->left;
            delete tmp;
            tmp = nullptr;
            return true;
        }
        return false;
    }
    bool isLeftSon(Node *tmp) { return tmp->parent && tmp == tmp->parent->left; }
    bool isRightSon(Node *tmp) { return tmp->parent && tmp == tmp->parent->right; }
    bool EraseWithoutChilds(Node *tmp)
    {
        if (!tmp)
        {
            return false;
        }
        if (!tmp->left && !tmp->right)
        {
            if (tmp == tmp->parent->right)
            {
                tmp->parent->right = nullptr;
            }
            else if (tmp == tmp->parent->left)
            {
                tmp->parent->left = nullptr;
            }
            delete tmp;
            return true;
        }
        return false;
    }
    bool EraseWithTwoChilds(Node *&tmp)
    {
        if (!tmp)
        {
            return false;
        }
        if (PrepairEraseLeftRight(tmp))
        {
            if (EraseWithOneChild(tmp))
            {
                return true;
            }
            else
            {
                EraseWithoutChilds(tmp);
            }
            return true;
        }
        if (PrepairEraseRightLeft(tmp))
        {
            if (EraseWithOneChild(tmp))
            {
                return true;
            }
            else
            {
                EraseWithoutChilds(tmp);
            }
            return true;
        }
        return false;
    }
    void InsertRepaint(Node *current)
    {
        Node *grand = current->parent->parent;
        if (!grand)
        {
            return;
        }
        if (current->parent == grand->right && grand->left &&
            grand->right->color == Color::kRed &&
            grand->left->color == Color::kRed)
        {
            grand->left->color = Color::kBlack;
            current->parent->color = Color::kBlack;
        }
        else if (current->parent == grand->left && grand->right &&
                 grand->right->color == Color::kRed &&
                 grand->left->color == Color::kRed)
        {
            grand->right->color = Color::kBlack;
            current->parent->color = Color::kBlack;
        }
        if (grand != root && grand->left && grand->left->color != Color::kRed &&
            grand->right && grand->right->color != Color::kRed)
        {
            grand->color = Color::kRed;
        }
    }
    void RepaintAfterErase(Node *parent, Node *left_child, Node *right_child)
    {
        // Балансировку производим от родитиля удаляемого узла
        if (!parent)
        {
            return;
        }

        if (parent->color == Color::kRed && left_child &&
            left_child == parent->left &&
            left_child->color == Color::kRed)
        {
            parent->left->color = Color::kBlack;
        }
        else if (parent->color == Color::kRed && right_child &&
                 right_child == parent->right &&
                 right_child->color == Color::kRed)
        {
            parent->right->color = Color::kBlack;
        }
        if (parent->color == Color::kRed && parent->right &&
            parent->right->color == Color::kBlack && parent->right->right &&
            parent->right->left &&
            parent->right->right->color == Color::kBlack &&
            parent->right->left->color == Color::kBlack)
        {
            parent->color = Color::kBlack;
            parent->right->color == Color::kRed;
        }
        else if (parent->color == Color::kRed && parent->left &&
                 parent->left->color == Color::kBlack && parent->left->left &&
                 parent->left->right &&
                 parent->left->right->color == Color::kBlack &&
                 parent->left->left->color == Color::kBlack)
        {
            parent->color = Color::kBlack;
            parent->left->color == Color::kRed;
        }
    }
    bool EraseWithRedParent(Node *tmp)
    {
        if (tmp->color == Color::kRed && tmp->left &&
            tmp->left->color == Color::kBlack && tmp->left->left &&
            tmp->left->left->color == Color::kRed)
        {
            Node *grand = tmp->parent;
            Node *left_brother_child = tmp->left->right;
            // Перестраиваем дерево
            tmp->left->right = tmp;
            if (isRightSon(tmp))
            {
                tmp->parent->right = tmp->left;
                tmp->parent = grand->right;
                grand->right->parent = grand;
                grand->right->color = Color::kRed;
                grand->right->left->color = Color::kBlack;
                grand->right->right->color = Color::kBlack;
            }
            else if (isLeftSon(tmp)) // Проверяем для того чтобы избежать выхода за корень дерева
            {
                tmp->parent->left = tmp->left;
                tmp->parent = grand->left;
                grand->left->parent = grand;
                grand->left->color = Color::kRed;
                grand->left->left->color = Color::kBlack;
                grand->left->right->color = Color::kBlack;
            }
            // Меняем родителя узла которого запомнили
            tmp->left = left_brother_child;
            if (left_brother_child)
            {

                tmp->left->parent = tmp;
            }
            return true;
        }
        else if (tmp->color == Color::kRed && tmp->right &&
                 tmp->right->color == Color::kBlack && tmp->right->right &&
                 tmp->right->right->color == Color::kRed)
        {
            Node *grand = tmp->parent;
            Node *right_brother_child = tmp->right->left;

            tmp->right->left = tmp;
            if (isRightSon(tmp))
            {
                tmp->parent->right = tmp->right;
                tmp->parent = grand->right;
                grand->right->parent = grand;
                grand->right->color = Color::kRed;
                grand->right->left->color = Color::kBlack;
                grand->right->right->color = Color::kBlack;
            }
            else if (isLeftSon(tmp)) // Проверяем для того чтобы избежать выхода за корень дерева
            {
                tmp->parent->left = tmp->right;
                tmp->parent = grand->left;
                grand->left->parent = grand;
                grand->left->color = Color::kRed;
                grand->left->left->color = Color::kBlack;
                grand->left->right->color = Color::kBlack;
            }
            tmp->right = right_brother_child;

            if (right_brother_child)
            {
                tmp->right->parent = tmp;
            }
            return true;
        }
        return false;
    }
    bool EraseWithBlackParentAndRedChild(Node *tmp)
    {
        Node *parent = tmp->parent;
        if (tmp->color == Color::kBlack && tmp->left &&
            tmp->left->color == Color::kRed && tmp->left->right &&
            tmp->left->right->color == Color::kBlack)
        {

            Node *left_right_child = tmp->left->right;
            // Перестраиваем дерево
            tmp->left->right = tmp;
            if (isRightSon(tmp))
            {
                tmp->parent->right = tmp->left;
                tmp->parent = parent->right;
                parent->right->parent = parent;
                parent->right->color = Color::kBlack;
                parent->right->left->color = Color::kRed;
                tmp->left = left_right_child;
            }
            else if (isLeftSon(tmp))
            {
                tmp->parent->left = tmp->left;
                tmp->parent = parent->left;
                parent->left->parent = parent;
                parent->left->color = Color::kBlack;
                parent->left->right->color = Color::kRed;
                tmp->left = left_right_child;
            }
            else
            {
                root = tmp->left;
                root->right->parent = root;
                root->parent = nullptr;
                root->right->left = left_right_child;
                root->color = Color::kBlack;
                root->right->left->color = Color::kRed;
            }
            // Меняем родителя узла которого запомнили
            tmp->left->parent = tmp;
            return true;
        }
        else if (tmp->color == Color::kBlack && tmp->right &&
                 tmp->right->color == Color::kRed && tmp->right->left &&
                 tmp->right->left->color == Color::kBlack)
        {
            Node *right_left_child = tmp->right->left;
            // Перестраиваем дерево
            tmp->right->left = tmp;
            if (isRightSon(tmp))
            {
                tmp->parent->right = tmp->right;
                tmp->parent = parent->right;
                parent->left->parent = parent;
                parent->left->color = Color::kBlack;
                parent->left->right->color = Color::kRed;
                tmp->right = right_left_child;
            }
            else if (isLeftSon(tmp))
            {
                tmp->parent->left = tmp->right;
                tmp->parent = parent->left;
                parent->right->parent = parent;
                parent->right->color = Color::kBlack;
                parent->right->left->color = Color::kRed;
                tmp->right = right_left_child;
            }
            else
            {
                root = tmp->right;
                root->left->parent = root;
                root->parent = nullptr;
                root->left->right = right_left_child;
                root->color = Color::kBlack;
                root->left->right->color = Color::kRed;
            }
            tmp->right->parent = tmp;
            return true;
        }
        return false;
    }
    void RotateAfterErase(Node *parent)
    {
        Node *tmp = parent;
        if (!tmp)
        {
            return;
        } // Так как узел мы удалили, то нужно производить балансировку от его родителя
        if (EraseWithRedParent(tmp))
        {
            return;
        }
        if (EraseWithBlackParentAndRedChild(tmp))
        {
            return;
        }     
    }

    void SubErase(int value)
    {
        Node *tmp = root;
        while (tmp)
        {
            if (tmp->value > value)
            {
                tmp = tmp->left;
            }
            else if (tmp->value < value)
            {
                tmp = tmp->right;
            }
            else if (tmp->value == value)
            {
                Node *tmp_parent = tmp->parent;
                Color tmp_color = tmp->color;
                Node *left_child = tmp->left;
                Node *right_child = tmp->right;
                /*запоминаем левого и правого ребенка удаляемого узла,
                для того чтобы определить перекраску*/
                if (EraseWithoutChilds(tmp))
                {
                }
                else if (EraseWithOneChild(tmp))
                {
                }
                else
                {
                    EraseWithTwoChilds(tmp);
                }
                if (tmp_color == Color::kRed)
                {
                    break; // Если удаляемый узел был красный, то значит черная высота не изменилась
                }
                else
                { // Иначе удаляемый узел был черным и высота изменилась
                    RepaintAfterErase(tmp_parent, left_child, right_child);
                    RotateAfterErase(tmp_parent);
                    break;
                }
            }
        }
    }

public:
    void Insert(int value)
    {
        if (!root)
        { // Если дерева не существует
            root = new Node(value, root);
            root->color = Color::kBlack;
            return;
        }
        Node *insert_node = SubInsert(value);
        while (insert_node && insert_node != root)
        {
            InsertRepaint(insert_node);
            InsertRotate(insert_node);
            if (!insert_node->parent)
            {
                break;
            }
            insert_node = insert_node->parent->parent;
        }
    }

    std::vector<InfoTree> GetTreeLikePreorderArray()
    {
        std::vector<InfoTree> info;
        Preorder(root, info);
        return info;
    }

    void Erase(int value)
    {
        if (!root)
        {
            return;
        }
        SubErase(value);
    }
    bool Find(int value)
    {
        Node *current = root;
        if (!root)
        {
            return false;
        }
        while (current != nullptr)
        {
            if (value < current->value)
            {
                if (current->left != nullptr)
                {
                    current = current->left;
                }
                else
                {
                    return false;
                }
            }
            if (value > current->value)
            {
                if (current->right != nullptr)
                {
                    current = current->right;
                }
                else
                {
                    return false;
                }
            }
            if (value == current->value)
            {
                return true;
            }
        }
        return false;
    }
};
