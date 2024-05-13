#include <iostream>
#include "RB_tree.h"
#include <string>
int main() {
    std::cout << "Hello" << std::endl;
    RBtree a;
    a.insert(3);
    a.insert(12);
    a.insert(16);
    a.insert(1);
    a.insert(5);
    a.print();
    return 0;
}