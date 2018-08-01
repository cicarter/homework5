#include "b_tree.h"
#include "b_tree_node.h"

int main()
{
    BTree<int> b3;
    for(int i = 0; i < 100; i++)
        b3.insert(i);
    b3.print();
}
