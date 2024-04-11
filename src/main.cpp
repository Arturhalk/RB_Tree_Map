#include <iostream>
#include "RB_tree.h"

int main() {
    std::cout << 12 << std::endl;
    RBTree a;
    a.insert(3);
    a.insert(12);
    a.insert(1);
    a.insert(5);
    std::cout << a.find(-30);
    return 0;
}