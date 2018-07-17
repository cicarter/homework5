#include "b_tree.h"

template <class dataType>
BTree<dataType>::BTree(int M)
{
    order = M;
    Root = NULL;
}

template <class dataType>
BTree<dataType>::BTree(const BTree& obj)
{
    order = obj.order;
    Root = obj.root;
}

template <class dataType>
BTree<dataType>::~BTree()
{
    //TODO add destructor
}

template <class dataType>
int BTree<dataType>::getOrder() const
{
    return order;
}

template <class dataType>
BTNode<dataType>* BTree<dataType>::getRoot() const
{
    return Root;
}

template <class dataType>
void BTree<dataType>::setRoot(BTNode<dataType>* root)
{
    Root = root;
}

template <class dataType>
BTNode<dataType>* BTree<dataType>::search(dataType val)
{
    return searchHelper(val, Root);
}

template <class dataType>
BTNode<dataType>* BTree<dataType>::searchHelper(dataType val, BTNode<dataType>* curr)
{
    //Terminating Condition 1: Node is NULL
    if(curr == NULL)
        return NULL;

    //TODO make a binary search function
    for(int i = 0; i < order - 1; i++)
    {
        dataType* arr = curr -> getDataArray();
        if(arr[i] == val)
            return curr;
        //Early Stopping
        else if(arr[i] > val)
            return searchHelper(val, curr -> getChildren[i]);
    }

}

template <class dataType>
void BTree<dataType>::insertHelper(dataType val, BTNode* currRoot)
{
    //Try to insert into root node by checking if space is avalible
    if(!currRoot -> isFull())
    {
        //Insert into array
        int i
        for(i = curr -> getDataArraySize() - 1; val < arr[i]; i--)
           arr[i + 1] = arr[i];
        arr[++i] = val;

        /*
            Test for Loop: M = 5, inserting: 3
            1 2 4
            i = curr -> getDataArraySize() - 1 = 3 - 1 = 2
            val = 3
            val = 3 < arr[i = 2] = 4 // Condition Met
            arr[i + 1 = 3] = arr[i = 2] // 1 2 4 4
            i--: i = 1
            val = 3 > arr[i = 1] = 2 // Condition Failed
            arr[++i: 2] = val // 1 2 3 4
        */

     }

        //Update dataArraySize
        curr -> incDataArraySize();
}
