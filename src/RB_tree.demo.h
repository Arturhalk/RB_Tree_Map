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
               // current->parent->color == Color::kRed &&
               (current->parent->color == Color::kRed || current->parent == root) &&
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
        if(right_current){
            right_current->parent = parent;
        }
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
        if(left_current){
            left_current->parent = parent;
        }
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
    Node *FindNode(int value)
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
    bool PrepairEraseLeftRight(Node *tmp,Color &color_deleted_node)
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
            lr_parent->parent = lr->left;
            // сменим родителя у удаляемого
            tmp->parent = lr_parent;
            // сменим родителей у заменяемого
            lr->left->parent = lr;
            lr->right->parent = lr;
            // меняем цвета
            Color color_tmp = tmp->color;
            tmp->color = lr->color;
            color_deleted_node = lr->color;
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
            left_curr->parent = tmp->parent;
            left_curr->right->parent = left_curr;
            tmp->parent = left_curr;
            if (left_left_son)
            {
                left_left_son->parent = left_curr->left;
            }
            // меняем цвета
            Color color_tmp = tmp->color;
            tmp->color = left_curr->color;
            color_deleted_node = left_curr->color;
            left_curr->color = color_tmp;
             return true;
        }
        return false;
    }
    bool PrepairEraseRightLeft(Node *tmp, Color &color_deleted_node)
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
            color_deleted_node = rl->color;
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
            right_curr->parent = tmp->parent;
            tmp->parent = right_curr;
            if (right_right_son)
            {
                right_right_son->parent = right_curr->right;
            }
            // меняем цвета
            Color color_tmp = tmp->color;
            tmp->color = right_curr->color;
            color_deleted_node = right_curr->color;
            right_curr->color = color_tmp;
            return true;
        }
        return false;
    }
    bool isLeftSon(Node *tmp) { return tmp->parent && tmp == tmp->parent->left; }
    bool isRightSon(Node *tmp) { return tmp->parent && tmp == tmp->parent->right; }
    bool isEraseWithOneChild(Node *&tmp)
    {
        if (!tmp)
        {
            return false;
        }
        if (!tmp->left && tmp->right)
        {
            return true;
        }
        if (tmp->left && !tmp->right)
        {
            return true;
        }
        return false;
    }
    Color EraseWithOneChild(Node *&tmp)
    {
        if (!tmp)
        {
            return Color::kRed;
        }
        if (!tmp->left && tmp->right)
        {
            Color right_node_color = tmp->right->color;
            tmp->right->parent = tmp->parent;
            if (isRightSon(tmp))
            {
                tmp->parent->right = tmp->right;
                tmp->parent->right->color = tmp->color;
            }
            else
            {
                tmp->parent->left = tmp->right;
                tmp->parent->left->color = tmp->color;
            }
            delete tmp;
            tmp = nullptr;
            return right_node_color;
        }
        if (tmp->left && !tmp->right)
        {
            Color left_node_color = tmp->left->color;
            tmp->left->parent = tmp->parent;
            if (isRightSon(tmp))
            {
                tmp->parent->right = tmp->left;
            }
            else
            {
                tmp->parent->left = tmp->left;
            }
            delete tmp;
            tmp = nullptr;
            return left_node_color;
        }
        return Color::kRed;
    }
    bool EraseWithoutChilds(Node *&tmp)
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
    Color EraseWithTwoChilds(Node *&tmp, Color erase_tmp_color)
    {
        if (!tmp)
        {
            return Color::kRed;
        }
        if (PrepairEraseLeftRight(tmp,erase_tmp_color))
        {
            if (EraseWithoutChilds(tmp))
            {
                return erase_tmp_color;
            }
            else
            {
                Color cur = EraseWithOneChild(tmp);
                return cur;
            }
        }
        else if (PrepairEraseRightLeft(tmp,erase_tmp_color))
        {
            if (EraseWithoutChilds(tmp))
            {
                return erase_tmp_color;
            }
            else
            {
                Color cur = EraseWithOneChild(tmp);
                return cur;
            }
        }
        return Color::kRed;
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
    bool RotateEraseBlackUncleAndRedChild(Node *tmp_parent, Node *uncle)
    {
        Node *grand = tmp_parent->parent;
        if (uncle->color == Color::kBlack &&
            uncle == tmp_parent->left && uncle->left &&
            uncle->left->color == Color::kRed)
        {
            Node *uncle_son_right = uncle->right;
            Node *parent_uncle = tmp_parent;
            Color tmp_parent_color = tmp_parent->color;
            if (isRightSon(tmp_parent))
            {
                // меняем расположение
                grand->right = uncle;
                uncle->right = tmp_parent;
                tmp_parent->left = uncle_son_right;
                // родители
                uncle->parent = grand;
                tmp_parent->parent = uncle;
                // цвета
                grand->right->color = tmp_parent_color;
                uncle->left->color = Color::kBlack;
                uncle->right->color = Color::kBlack;
                return true;
            }
            else if (isLeftSon(tmp_parent))
            {
                // меняем расположение
                grand->left = uncle;
                uncle->right = tmp_parent;
                tmp_parent->left = uncle_son_right;
                // родители
                uncle->parent = grand;
                tmp_parent->parent = uncle;

                // цвета
                uncle->right->color = Color::kBlack;
                grand->left->color = tmp_parent_color;
                uncle->left->color = Color::kBlack;
                return true;
            }
            else
            {
                root = uncle;
                root->right = tmp_parent;
                tmp_parent->left = uncle_son_right;

                uncle->parent = nullptr;
                tmp_parent->parent = uncle;

                root->color = Color::kBlack;
                root->left->color = Color::kBlack;
                root->right->color = Color::kBlack;
                return true;
            }
            if (uncle_son_right)
            {
                uncle_son_right->parent = tmp_parent;
            }
        }
        else if (uncle->color == Color::kBlack &&
                 uncle == tmp_parent->right && uncle->right &&
                 uncle->right->color == Color::kRed)
        {
            Node *uncle_son_left = uncle->left;
            Node *parent_uncle = tmp_parent;
            Color tmp_color = tmp_parent->color;
            if (isRightSon(tmp_parent))
            {
                // меняем расположение
                grand->right = uncle;
                uncle->left = tmp_parent;
                tmp_parent->right = uncle_son_left;
                // родители
                uncle->parent = grand;
                tmp_parent->parent = uncle;
                // цвета
                grand->right->color = tmp_color;
                uncle->left->color = Color::kBlack;
                uncle->right->color = Color::kBlack;
                return true;
            }
            else if (isLeftSon(tmp_parent))
            {
                // меняем расположение
                grand->left = uncle;
                uncle->left = tmp_parent;
                tmp_parent->left = uncle_son_left;
                // родители
                uncle->parent = grand;
                tmp_parent->parent = uncle;

                // цвета
                grand->left->color = tmp_color;
                uncle->left->color = Color::kBlack;
                uncle->right->color = Color::kBlack;
                return true;
            }
            else
            {
                root = uncle;
                root->left = tmp_parent;
                tmp_parent->right = uncle_son_left;

                uncle->parent = nullptr;
                tmp_parent->parent = uncle;

                root->color = Color::kBlack;
                root->left->color = Color::kBlack;
                root->right->color = Color::kBlack;
                return true;
            }
            if (uncle_son_left)
            {
                uncle_son_left->parent = tmp_parent;
            }
        }
        return false;
    }

    bool RotateEraseBlackUncleBlackChildRedChild(Node *tmp_parent, Node *uncle)
    {
        Node *grand = tmp_parent->parent;
        Node *parent_ = tmp_parent;
        if ((uncle->color == Color::kBlack &&
             uncle->left && uncle->left->color == Color::kBlack &&
             uncle->right && uncle->right->color == Color::kRed) ||
            (uncle->color == Color::kBlack &&
             uncle->right && uncle->right->color == Color::kRed))
        {
            Color color_tmp = tmp_parent->color;
            if (isRightSon(tmp_parent))
            {
                grand->right = uncle->right;
                grand->right->right = parent_;
                grand->right->right->left = nullptr;
                grand->right->left = uncle;
                uncle->right = nullptr;

                grand->right->parent = grand;
                grand->right->right->parent = grand->right;
                grand->right->left->parent = grand->right;

                grand->right->right->color = Color::kBlack;
                grand->right->color = color_tmp;
                return true;
            }
            else if (isLeftSon(tmp_parent))
            {
                grand->left = uncle->right;
                grand->left->right = parent_;
                grand->left->right->left = nullptr;
                grand->left->left = uncle;
                uncle->right = nullptr;

                grand->left->parent = grand;
                grand->left->right->parent = grand->left;
                grand->left->left->parent = grand->left;

                grand->left->right->color = Color::kBlack;
                grand->left->color = color_tmp;
                return true;
            }
            else
            {
                root = uncle->right;
                root->right = parent_;
                root->left = uncle;
                root->right->left = nullptr;
                uncle->right = nullptr;

                root->parent = nullptr;
                root->right->parent = root;
                root->left->parent = root;

                root->color = Color::kBlack;
                root->right->color = Color::kBlack;
                return true;
            }
        }
        else if ((uncle->color == Color::kBlack &&
                  uncle->right && uncle->right->color == Color::kBlack &&
                  uncle->left && uncle->left->color == Color::kRed) ||
                 (uncle->color == Color::kBlack &&
                  uncle->left && uncle->left->color == Color::kRed))
        {
            Color color_tmp = tmp_parent->color;
            if (isRightSon(tmp_parent))
            {
                grand->right = uncle->left;
                grand->right->right = uncle;
                grand->right->left = parent_;
                grand->right->left->right = nullptr;
                uncle->left = nullptr;

                grand->right->parent = grand;
                grand->right->right->parent = grand->right;
                grand->right->left->parent = grand->right;

                grand->right->left->color = Color::kBlack;
                grand->right->color = color_tmp;
                return true;
            }
            else if (isLeftSon(tmp_parent))
            {
                grand->left = uncle->left;
                grand->left->right = uncle;
                grand->left->left = parent_;
                grand->left->left->right = nullptr;
                uncle->left = nullptr;

                grand->left->parent = grand;
                grand->left->right->parent = grand->right;
                grand->left->left->parent = grand->right;

                grand->left->left->color = Color::kBlack;
                grand->left->color = color_tmp;
                return true;
            }
            else
            {
                root = uncle->right;
                root->right = parent_;
                root->left = uncle;
                root->right->left = nullptr;
                uncle->right = nullptr;

                root->parent = nullptr;
                root->right->parent = root;
                root->left->parent = root;

                root->color = Color::kBlack;
                root->right->color = Color::kBlack;
                return true;
            }
        }
        return false;
    }

    bool RotateEraseRedUncleWithBlackChilds(Node *tmp_parent, Node *uncle)
    {
        Node *grand = tmp_parent->parent;
        if (uncle == tmp_parent->left && uncle->color == Color::kRed &&
            uncle->left &&
            uncle->left->color == Color::kBlack && uncle->right &&
            uncle->right->color == Color::kBlack)
        {
            Node *uncle_child_right = uncle->right;
            if (isRightSon(tmp_parent))
            {
                grand->right = uncle;
                grand->right->right = tmp_parent;
                tmp_parent->left = uncle_child_right;

                tmp_parent->parent = uncle;
                uncle->parent = grand;

                uncle->color = Color::kBlack;
                tmp_parent->color = Color::kBlack;
            }
            else if (isLeftSon(tmp_parent))
            {

                grand->left = uncle;
                grand->left->right = tmp_parent;
                tmp_parent->left = uncle_child_right;

                tmp_parent->parent = uncle;
                uncle->parent = grand;

                uncle->color = Color::kBlack;
                tmp_parent->color = Color::kBlack;
            }
            else
            {
                root = uncle;
                root->right = tmp_parent;
                tmp_parent->left = uncle_child_right;

                root->left->parent = root;
                root->right->parent = root;
                tmp_parent->left->parent = tmp_parent;

                root->color = Color::kBlack;
                root->right->color = Color::kBlack;
            }
            uncle_child_right->parent = tmp_parent;
            uncle_child_right->color = Color::kRed;
            return true;
        }
        else if (uncle == tmp_parent->right && uncle->color == Color::kRed &&
                 uncle->right &&
                 uncle->right->color == Color::kBlack && uncle->left &&
                 uncle->left->color == Color::kBlack)
        {
            Node *uncle_child_left = uncle->left;
            if (isRightSon(tmp_parent))
            {
                grand->right = uncle;
                grand->right->left = tmp_parent;
                tmp_parent->right = uncle_child_left;

                tmp_parent->parent = uncle;
                uncle->parent = grand;

                uncle->color = Color::kBlack;
                tmp_parent->color = Color::kBlack;
            }
            else if (isLeftSon(tmp_parent))
            {

                grand->left = uncle;
                grand->left->left = tmp_parent;
                tmp_parent->right = uncle_child_left;

                tmp_parent->parent = uncle;
                uncle->parent = grand;

                uncle->color = Color::kBlack;
                tmp_parent->color = Color::kBlack;
            }
            else
            {
                root = uncle;
                root->left = tmp_parent;
                tmp_parent->right = uncle_child_left;

                root->left->parent = root;
                root->right->parent = root;
                tmp_parent->right->parent = tmp_parent;

                root->color = Color::kBlack;
                root->left->color = Color::kBlack;
            }
            uncle_child_left->parent = tmp_parent;
            uncle_child_left->color = Color::kRed;
            return true;
        }
        return false;
    }

    bool RotateEraseBlackUncleWithBlackChilds(Node *tmp_parent, Node *uncle)
    {
        if (uncle->color == Color::kBlack && uncle->right &&
            uncle->right->color == Color::kBlack && uncle->left &&
            uncle->left->color == Color::kBlack)
        {
            uncle->color = Color::kRed;
            tmp_parent->color = Color::kBlack;
            return true;
        }
        return false;
    }

    void RotateAfterErase(Node *tmp_parent, Node *uncle)
    {
        if (!tmp_parent)
        {
            return;
        }
        Color tmp_parent_color = tmp_parent->color;
        if (uncle && uncle->color == Color::kBlack)
        {
            if (RotateEraseBlackUncleWithBlackChilds(tmp_parent, uncle) && tmp_parent_color == Color::kBlack)
            {
                while (tmp_parent != root && tmp_parent->color == Color::kBlack)
                {
                    if (RotateEraseBlackUncleAndRedChild(tmp_parent, uncle))
                    {
                    }
                    if (RotateEraseBlackUncleBlackChildRedChild(tmp_parent, uncle))
                    {
                    }
                    if (RotateEraseBlackUncleWithBlackChilds(tmp_parent, uncle))
                    {
                    }
                    tmp_parent = tmp_parent->parent;
                    uncle = uncle->parent;
                }
            }
            else
            {
                if (RotateEraseBlackUncleAndRedChild(tmp_parent, uncle))
                {
                    return;
                }
                if (RotateEraseBlackUncleBlackChildRedChild(tmp_parent, uncle))
                {
                    return;
                }
            }
        }
        else if (uncle && uncle->color == Color::kRed)
        {
            RotateEraseRedUncleWithBlackChilds(tmp_parent, uncle);
        }
    }
    void SubErase(Node* tmp){
        Node *tmp_parent = tmp->parent;
        Node *tmp_uncle = nullptr;
        Color tmp_color = tmp->color;
        if (isRightSon(tmp))
        {
            tmp_uncle = tmp_parent->left;
        }
        else if (isLeftSon(tmp))
        {
            tmp_uncle = tmp_parent->right;
        }

        if (EraseWithoutChilds(tmp))
        {
            if (tmp_color == Color::kRed)
            {
                return;
            }
        }
        else if (isEraseWithOneChild(tmp))
        {
            Color cur = EraseWithOneChild(tmp);
            if(cur == Color::kRed){
                return;
            }  
        }
        else
        {
            Color cur = EraseWithTwoChilds(tmp, tmp_color);
            if(cur == Color::kRed){
                return;
            }
        }
        RotateAfterErase(tmp_parent, tmp_uncle);
    }
    void FindNodeForErase(int value)
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
                SubErase(tmp);
                break;
            }
        }
    }

public:
    void
    Insert(int value)
    {
        if (!root)
        { // Если дерева не существует
            root = new Node(value, root);
            root->color = Color::kBlack;
            return;
        }
        Node *insert_node = FindNode(value);
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
        FindNodeForErase(value);
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
