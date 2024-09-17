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
        // Меняем зависимости снова)
        tmp->parent = grand;
        parent->parent = tmp;
        parent->left = right_current;
        // Переходим к разбалансировке RR
        RotateRightRight(tmp->right);
    }
    void RotateLeftRight(Node *tmp)
    {
        Node *grand = tmp->parent->parent; // 12
        Node *parent = tmp->parent;        // 3
        Node *left_current = tmp->left;
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
            // сменим родителя у удаляемого
            tmp->parent = lr_parent;
            // меняем цвета
            Color color_tmp = tmp->color;
            tmp->color = lr->color;
            lr->color = color_tmp;
            // сохраняем новое дерево
            if (root == tmp)
            {
                root = &(*lr);
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

            tmp->parent = rl_parent;

            Color color_tmp = tmp->color;
            tmp->color = rl->color;
            rl->color = color_tmp;

            root = &(*rl);
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
    bool isLeftSon(Node *tmp) { return tmp == tmp->parent->left; }
    bool isRightSon(Node *tmp) { return tmp == tmp->parent->right; }
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
    void EraseRepaint(Node *parent)
    {
        if (parent->color == Color::kRed && parent->right &&
            parent->right->color == Color::kBlack && parent->right->right &&
            parent->right->right->color == Color::kBlack &&
            parent->right->left->color == Color::kBlack)
        {
            parent->color = Color::kBlack;
            parent->right->color == Color::kRed;
        }
        else if (parent->color == Color::kRed && parent->left &&
                 parent->left->color == Color::kBlack && parent->left->left &&
                 parent->left->right->color == Color::kBlack &&
                 parent->left->left->color == Color::kBlack)
        {
            parent->color = Color::kBlack;
            parent->left->color == Color::kRed;
        }
    }
    bool isEraseWithRedParent(Node *tmp)
    {
        Node *grand = tmp->parent;
        if (tmp->color == Color::kRed && tmp->left &&
            tmp->left->color == Color::kBlack && tmp->left->left &&
            tmp->left->left->color == Color::kRed)
        {
            // Запоминаем правого внука сына
            Node *left_brother_child = tmp->left->right;
            // Перестраиваем
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
            else
            {
                tmp->parent->left = tmp->left;
                tmp->parent = grand->left;
                grand->left->parent = grand;
                grand->left->color = Color::kRed;
                grand->left->left->color = Color::kBlack;
                grand->left->right->color = Color::kBlack;
            }
            // Меняем родителей и зависимости
            tmp->left = left_brother_child;
            if (left_brother_child)
            {

                tmp->left->parent = tmp;
                left_brother_child->parent = tmp;
            }
            return true;
        }
        else if (tmp->color == Color::kRed && tmp->right &&
                 tmp->right->color == Color::kBlack && tmp->right->right &&
                 tmp->right->right->color == Color::kRed)
        {

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
            else
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
                right_brother_child->parent = tmp;
            }
            return true;
        }
        return false;
    }
    void EraseRotate(Node *parent)
    {
        Node *tmp = parent; // Возьмём родителя как данный узел
        if (isEraseWithRedParent(tmp))
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
                    return;
                }
                else if (tmp_color == Color::kBlack)
                {
                    while (tmp_parent != root)
                    { // начинаем от отца
                        EraseRepaint(tmp_parent);
                        EraseRotate(tmp_parent);
                        if (!tmp->parent)
                        {
                            break;
                        }
                        tmp_parent = tmp_parent->parent;
                    }
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
