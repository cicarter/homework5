#ifndef B_TREE_H
#define B_TREE_H

/* Chris' Notes

    While I tried using normal syntax for making the arrays (no DMA)
    Compilation failed. So now all the node memory is allocated to the heap
    Management of pointers and freeing memory is critical

*/

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
        //Constructors & Destructors
        BTNode(int order_);
        BTNode(const BTNode& from);
        ~BTNode();

        //Getters
        DataType* getDataArray() const;
        BTNode** getChildren() const;

        //Setters
        void setDataArray(DataType* arr);
        void setChildren(BTNode** childen);

        //Methods
        bool isFull() const;
        bool isEmpty() const;
        bool isLeaf() const;

    private:
        int order;
        int keys;
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
    for(int i = 0; i < order; i++)
        if (children[i] != NULL)
            return false;
    return true;
}

#endif
