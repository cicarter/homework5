#ifndef B_TREE_H
#define B_TREE_H

/* Chris' Notes

    While I tried using normal syntax for making the arrays (no DMA)
    Compilation failed. So now all the node memory is allocated to the heap
    Management of pointers and freeing memory is critical

*/

// I added the "keys" variable, which keeps track of the number of keys in each node.
// Make sure in your methods to only have/look for data in the array spots from 0 to keys - 1
// Without doing things like this I could not implement a valid remove function
// - Davis


/* B_Tree Node in a nutshell

    Each Node is an array of size order - 1, where order is now many
    children / branches a node could have

    The order of a tree should not be changed, else the tree has
    to be redone.

    The constructor allocates memory of the data Array on the heap

    Tip: I don't know if others do this, but when I add an element
    to a data structure, usually I create an instance of a node,
    assign it whatever value and add that node to the structure

    I would discourage doing this because each node is an array,
    so one node may not be full

*/

//Using a template format for flexablity
template <class DataType>
class BTNode{
    public:
        //Constructors
        BTNode(int order_);

        // Copy constructor
        BTNode(const BTNode& from);

        // Deconstructor
        ~BTNode();
/*
// These are not needed now that the BTree class can access the private variables
        //Getters
        DataType* getDataArray() const;
        BTNode** getChildren() const;
        int getKeys() const;

        //Setters
        void setDataArray(DataType* arr);
        void setChildren(BTNode** childen);
        void setKeys(int keys_);
*/
        //Methods
        bool isFull() const;
        bool isEmpty() const;
        bool isLeaf() const;
        void remove(DataType value_);

        template <class dataType>
        friend class BTree;

    private:
        int order;
        int keys;
        bool leaf;
        DataType* dataArray;
        int dataArraySize;
        BTNode** children;
        int childrenArraySize;
};

#include "b_tree_node.h"
#include <iostream>

using namespace std;

template <class DataType>
BTNode<DataType>::BTNode(int order_)
{
    dataArray = new DataType[order_];
    children = new BTNode*[order_];
    order = order_;
    keys = 0;
    leaf = true;
    for (int i = 0; i < order_ + 1; i++)
        children[i] = NULL;
}

template <class DataType>
BTNode<DataType>::BTNode(const BTNode &obj)
{
    //Implement a deep copy
}

template <class DataType>
BTNode<DataType>::~BTNode()
{
    delete[] dataArray;
    delete[] children;
}
/*
template <class DataType>
DataType* BTNode<DataType>::getDataArray() const
{
    return dataArray;
}

template <class DataType>
BTNode<DataType>** BTNode<DataType>::getChildren() const
{
    return children;
}

template <class DataType>
void BTNode<DataType>::setDataArray(DataType* arr)
{
    for(int i = 0; i < order - 1; i++)
        dataArray[i] = arr[i];
}

template <class DataType>
void BTNode<DataType>::setChildren(BTNode** Children)
{
    children = Children;
}
*/
template <class DataType>
bool BTNode<DataType>::isEmpty() const
{
    return dataArraySize == 0;
}

template <class DataType>
bool BTNode<DataType>::isFull() const
{
    return dataArraySize == order - 1;
}

//-------------------------------------------------
// Method: isLeaf
// Purpose: Helper method which checks if a node is a leaf node by looking though the children array to see if all are NULL
// Date: 07/28/18
// Author: Davis Campbell
//-------------------------------------------------
template <class DataType>
bool BTNode<DataType>::isLeaf() const
{
    return leaf;
}
/*
//-------------------------------------------------
// Method: getKeys
// Purpose: Return the number of keys in a given node
// Date: 07/29/18
// Author: Davis Campbell
//-------------------------------------------------
template <class DataType>
int BTNode<DataType>::getKeys() const
{
    return keys;
}

//-------------------------------------------------
// Method: setKeys
// Purpose: set the number of keys in a node
// Date: 07/29/18
// Author: Davis Campbell
//-------------------------------------------------
template <class DataType>
void BTNode<DataType>::setKeys(int keys_)
{
    keys = keys_;
}
*/
template <class DataType>
void BTNode<DataType>::remove(DataType value_)
{
    for(int i = 0; i < keys && dataArray[i] < value_; i++)

    if(dataArray[i] != value_)
    {
        if(isLeaf())
        {
            cout << "The value " << value_ << " is not in this tree.\n";
            return;
        }

        // From the logic, we need to fill the child node where the key is supposed to be if it's not full
        if (children[i]->keys < order)
        {
            // From the logic, if the next child node has more than order-1 keys we take a key from that child
            if (i != keys && children[i + 1]->keys > order - 1)
            {
                BTNode* First = children[i];
                BTNode* Next = children[i + 1];

                // copy the value at the i to the end of the First node
                First->dataArray[First->keys] = dataArray[i];

                // copy the first child from the Next node to the end of the first node
                if(!First->isLeaf())
                    First->children[First->keys + 1] = Next->children[0];

                // copy the first value of the Next node to position i
                dataArray[i] = Next->dataArray[0];

                // Move all the values back in the Next node
                for (int j = 0; j < Next->keys; j++)
                    Next->dataArray[j] = Next->dataArray[j + 1];

                // move all the children pointers back in the Next node if it is not a leaf
                if (!Next->isLeaf())
                    for (int k = 0; k < Next->keys; k++)
                        Next->children[k] = Next->children[k + 1];

                // change the number of keys in each changed node
                Next->keys = Next->keys - 1;
                First->keys = First->keys + 1;
            }

            // From the logic, if the previous child node has more than order - 1 keys, take a key from that child
            else if (i != 0 && children[i - 1]->keys > order - 1)
            {
                BTNode* First = children[i];
                BTNode* Previous = children[i - 1];

                // move all the data in First to the right
                for (int j = First->keys; j > -1; j--)
                    First->dataArray[j] = First->dataArray[j + 1];

                //move all the children over as well
                if (!First->isLeaf())
                    for(int j = First->keys; j > -1; j--)
                        First->children[j] = First->children[j + 1];

                // move the value at i to the first spot in First
                First->dataArray[0] = dataArray[i];

                // move the last value in previous to the spot at i
                dataArray[i] = Previous->dataArray[Previous->keys];

                // move the previous child over
                if (!First->isLeaf())
                    First->children[0] = Previous->children[Previous->keys + 1];

                // change number of keys in each
                First->keys = First->keys + 1;
                Previous->keys = Previous->keys - 1;
            }

            // From the logic, as a last option we merge it with its sibling.
            else
            {
                // From the logic, if it's the last child node, merge it with its predecessor, otherwise merge it with its successor
                if (i != keys)
                {
                    // merge with next sibling
                }
                else
                {
                    // merge with previous sibling
                }
            }
        }

        bool keyPresent = (i == keys? true : false);

        //
    }
    else if(dataArray[i] == value_)
    {

    }
}



#endif
