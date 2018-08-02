#include "b_tree.h"
#include <iostream>
#include <time.h>

using namespace std;
int main()
{
    /*
    No error checking on inputs for this, so user must be skilled lol
    */
    srand(time(NULL));
    int size; // this number can be literally as big as you want it to be haha
    int choice, value, degree, type, length;
    string word;
    char character;
    cout << "Define your B-Tree\n"
         << "Degree: ";
    cin >> degree;
    cout << "Choose data type:\n"
         << "(1) Integers\n"
         << "(2) Characters\n"
         << "(3) Strings\n"
         << "Your choice: ";
    cin >> type;

    BTree<int> ints(degree);
    BTree<char> chars(degree);
    BTree<string> strings(degree);

    while (choice != 6)
    {
        cout << "\nMenu:\n"
             << "(1) Insert a data point\n"
             << "(2) Search for a value\n"
             << "(3) Insert a bunch of random values\n"
             << "(4) Remove a value\n"
             << "(5) Print the tree\n"
             << "(6) Quit\n"
             << "Your choice: ";
        cin >> choice;

        if (choice == 1)
        {
            if (type == 1)
            {
                cout << "Number: ";
                cin >> value;
                ints.insert(value);
            }
            else if (type == 2)
            {
                cout << "Character: ";
                cin >> character;
                chars.insert(character);
            }
            else
            {
                cout << "Word: ";
                cin >> word;
                strings.insert(word);
            }
        }
        else if (choice == 2)
        {
            if (type == 1)
            {
                cout << "Number: ";
                cin >> value;
                if(ints.search(value) != NULL)
                    cout << "The value " << value << " was found in the tree.\n";
                else
                    cout << "The value " << value << " was NOT found in the tree.\n";
            }
            else if (type == 2)
            {
                cout << "Character: ";
                cin >> character;
                if(chars.search(character) != NULL)
                    cout << "The character " << character << " was found in the tree.\n";
                else
                    cout << "The character " << character << " was NOT found in the tree.\n";
            }
            else
            {
                cout << "Word: ";
                cin >> word;
                if(strings.search(word) != NULL)
                    cout << "The word " << word << " was found in the tree.\n";
                else
                    cout << "The word " << word << " was NOT found in the tree.\n";
            }
        }
        else if (choice == 3)
        {
            cout << "Enter number of values:";
            cin >> size;
            if (type == 1)
            {
                for (int i = 0; i < size; i++)
                    ints.insert(rand() % 1000);
            }
            else if (type == 2)
            {
                for (int i = 0; i < size; i++)
                    chars.insert('a' + rand() % 26);
            }
            else
            {
                cout << "Word Lengths: ";
                cin >> length;
                for (int i = 0; i < size; i++)
                {
                    word.clear();
                    for (int i = 0; i < length; i++)
                        word += 'a' + rand() % 26;
                    strings.insert(word);
                }
            }
        }
        else if (choice == 4)
        {
            if (type == 1)
            {
                cout << "Number: ";
                cin >> value;
                ints.remove(value);
            }
            else if (type == 2)
            {
                cout << "Character: ";
                cin >> character;
                chars.remove(character);
            }
            else
            {
                cout << "Word: ";
                cin >> word;
                strings.remove(word);
            }
        }
        else if (choice == 5)
        {
             if (type == 1)
            {
                ints.print();
            }
            else if (type == 2)
            {
                chars.print();
            }
            else
            {
                strings.print();
            }
        }
	else if (choice == 6)
	   return 0;
    }

}
