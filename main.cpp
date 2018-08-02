#include "b_tree.h"
#include <iostream>
#include <time.h>

using namespace std;
int main()
{
    const int size = 10; // this number can be literally as big as you want it to be haha
    BTree<int> test(4);

    srand(time(NULL));
    for (int i = 0; i < size; i++)
    {
        int j = rand() % 10;
        test.insert(j);
        test.print();
        cout << endl;
    }
    cout << endl;

    cout << "Initial\n";
    test.print();
    cout << endl;
    int choice = 0;
    while(choice != -1)
    {
        cout << "Enter choice to remove: ";
        cin >> choice;
        test.remove(choice);
        test.print();
        cout << endl;
    }

}
