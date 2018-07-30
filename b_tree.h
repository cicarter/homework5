#include "b_tree_node.h"
#include <cstdlib>

template <class dataType>
class BTree{

    public:
        BTree(int degree_);
        BTree(const BTree& obj);
        ~BTree();

        //Getters
        int getdegree() const; // needed? the person coding main would know this so they wouldn't need to check through a method?
        BTNode<dataType>* getRoot() const;

        //Setters
        void setRoot(BTNode<dataType>* Root); // needed?

        //Common Data Structures Methods
        BTNode<dataType>* search(dataType val);
        void insert(dataType val);
        void remove(dataType value_);
        void print() const;

    private:
        //Member Variables
        int degree;
        BTNode<dataType>* Root;

        //Helper Functinos
        BTNode<dataType>* searchHelper(dataType val, BTNode<dataType>* curr);
};

template <class dataType>
BTree<dataType>::BTree(int degree_)
{
    degree = degree_;
    Root = NULL;
}

template <class dataType>
BTree<dataType>::BTree(const BTree& obj)
{
    degree = obj.degree;
    Root = obj.root;
}

template <class dataType>
BTree<dataType>::~BTree()
{
    //TODO add destructor
}

template <class dataType>
int BTree<dataType>::getdegree() const
{
    return degree;
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
    for(int i = 0; i < degree - 1; i++)
    {
        dataType* arr = curr -> getDataArray();
        if(arr[i] == val)
            return curr;
        //Early Stopping
        else if(arr[i] > val)
            return searchHelper(val, curr -> getChildren[i]);
    }

}

/*
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


            Test for Loop: M = 5, inserting: 3
            1 2 4
            i = curr -> getDataArraySize() - 1 = 3 - 1 = 2
            val = 3
            val = 3 < arr[i = 2] = 4 // Condition Met
            arr[i + 1 = 3] = arr[i = 2] // 1 2 4 4
            i--: i = 1
            val = 3 > arr[i = 1] = 2 // Condition Failed
            arr[++i: 2] = val // 1 2 3 4


     }

        //Update dataArraySize
        curr -> incDataArraySize();
}
*/

//-------------------------------------------------
// Method: Remove
// Purpose: Remove an element from the B Tree
// Date: 07/28/18
// Author: Davis Campbell
//-------------------------------------------------
template <class dataType>
void BTree<dataType>::remove(dataType value_)
{
    if(Root == NULL)
    {
        cout << "Error: Can not delete from empty tree";
        return;
    }

    Root->remove(value_);

    // handle the case that the root is now empty
    if (Root->keys == 0)
    {
        BTNode<dataType>* temp = Root;
        if(Root->isLeaf())
            Root = NULL;
        else
            Root = Root->children[0];
        delete temp;
    }
}


