#include "b_tree.h"
#include <iostream>
#include <time.h>

using namespace std;
int main()
{
    const int size = 70; // this number can be literally as big as you want it to be haha
    BTree<int> test(3);

    srand(time(NULL));
    for (int i = 0; i < size; i++)
    {
        int j = rand() % 1000;
        test.insert(j);
    }
    cout << endl;

    cout << "Initial\n";
    test.print();
    cout << endl;
    int choice = 0;
    while(choice != -1)
    {
        cout << "Enter choice to remove (-1 for quit): ";
        cin >> choice;
        test.remove(choice);
        test.print();
        cout << endl;
    }

}
