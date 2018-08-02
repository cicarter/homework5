#include "b_tree.h"
#include <iostream>
#include <time.h>

using namespace std;
int main()
{
    const int size = 30; // this number can be literally as big as you want it to be haha
    BTree<int> test(3);

    srand(time(NULL));
    for (int i = 0; i < size; i++)
        test.insert2(rand() % 100);


    cout << "Initial\n";
    test.Print();
    cout << endl;
    int choice = 0;
    while(choice != -1)
    {
        cout << "Enter choice to remove: ";
        cin >> choice;
        test.remove(choice);
        test.Print();
        cout << endl;
    }

}
