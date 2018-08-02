//-------------------------------------------------
// FileName: b_tree.h
// Purpose: Define and implement the methods and data associated with the BTree class
// Date: 07/17/18
// Authors: Chris Carter, Davis Campbell, Wesley Nimmo, Sam Yeager
//-------------------------------------------------


#include "b_tree_node.h"
#include <cstdlib>

template <class dataType>
class BTree{

    public:
        BTree(int degree_);
        ~BTree();

        //Common Data Structures Methods
        BTNode<dataType>* search(dataType val);
        void insert(dataType value_);
        void remove(dataType value_);
        void print() const;

    private:
        //Member Variables
        int degree;
        BTNode<dataType>* Root;

        //Helper Functinos
        BTNode<dataType>* searchHelper(dataType val, BTNode<dataType>* curr);
        void printHelper(BTNode<dataType>* current) const;
        void insertHelper(dataType value_, BTNode<dataType>* current);
};

//-------------------------------------------------
// Method: Constructor
// Purpose: Initialize the private variables for the tree
// Date: 07/17/18
// Author: Chris Carter
//-------------------------------------------------
template <class dataType>
BTree<dataType>::BTree(int degree_)
{
    degree = degree_;
    Root = NULL;
}

//-------------------------------------------------
// Method: Deconstructor
// Purpose: free space from the tree
// Date: 08/01/18
// Author: Wesley Nimmo
//-------------------------------------------------
template <class dataType>
BTree<dataType>::~BTree()
{
    //TODO add destructor
}

//-------------------------------------------------
// Method: Search
// Purpose: Search for an element within the tree
// Date: 07/17/18
// Author: Chris Carter
//-------------------------------------------------
template <class dataType>
BTNode<dataType>* BTree<dataType>::search(dataType val)
{
    return searchHelper(val, Root);
}

//-------------------------------------------------
// Method: SearchHelper
// Purpose: Help to search for an element in the tree
// Date: 07/17/18
// Author: Chris Carter
//-------------------------------------------------

template <class dataType>
BTNode<dataType>* BTree<dataType>::searchHelper(dataType val, BTNode<dataType>* curr)
{
    //Terminating Condition 1: Node is NULL
    if(curr == NULL)
        return NULL;

    // binary search function
    for(int i = 0; i < (2 * degree) - 1; i++)
    {
        if(curr->dataArray[i] == val)
            return curr;
        //Early Stopping
        else if(curr->dataArray[i] > val)
            return searchHelper(val, curr -> getChildren[i]);
    }

}

//-------------------------------------------------
// Method: Insert
// Purpose: Insert an element into the B-Tree
// Date: 08/01/18
// Author: Davis Campbell
//-------------------------------------------------
template <class dataType>
void BTree<dataType>::insert(dataType value_)
{
    // if the tree is empty just make a new node
    if (Root == NULL)
    {
        Root = new BTNode<dataType>(degree, true);
        Root->dataArray[0] = value_;
        Root->keys = 1;
    }
    // if the tree is not empty
    else
    {
        // if the root is not full, run insertHelper to simply add to it
        if (!(Root->isFull()))
            insertHelper(value_, Root);
        // if it is full, we need to split it
        else
        {
        // splitting the root
            // making 2 new nodes for it to be split into
            BTNode<dataType>* newRoot = new BTNode<dataType>(degree, false);
            BTNode<dataType>* sibling = new BTNode<dataType>(degree, Root->leaf);

            // Moving data from the old root to the new sibling
            for (int j = 0; j < degree - 1; j++)
                sibling->dataArray[j] = Root->dataArray[j + degree];
            if (!(Root->leaf))
            {
                for (int j = 0; j < degree; j++)
                    sibling->children[j] = Root->children[j + degree];
            }

            // making space in the new root for the old median key
            for (int j = newRoot->keys; j >= 1; j--)
                newRoot->children[j+1] = newRoot->children[j];
            for (int j = newRoot->keys - 1; j >= 0; j--)
                newRoot->dataArray[j + 1] = newRoot->dataArray[j];

            newRoot->dataArray[0] = Root->dataArray[degree-1];

            // setting pointers and updating keys
            newRoot->children[0] = Root;
            newRoot->children[1] = sibling;
            Root->keys = degree - 1;
            sibling->keys = degree - 1;
            newRoot->keys++;
            Root = newRoot;

            // find the correct point at which we should look to insert
            int i = 0;
            if (newRoot->dataArray[0] < value_)
                i++;

            // Insert into the now for sure non-full node
            insertHelper(value_, newRoot->children[i]);
        }

    }
}

//-------------------------------------------------
// Method: Insert Helper
// Purpose: Insert an element into the B-Tree
// Date: 08/01/18
// Author: Davis Campbell
//-------------------------------------------------
template <class dataType>
void BTree<dataType>::insertHelper(dataType value_, BTNode<dataType>* current)
{
    // set an index to the last data point position of the current array
    int i = current->keys - 1;

    // if the current is a leaf, we just move over the values and put it in
    if (current->leaf)
    {
        // move everything over
        while (i >= 0 && current->dataArray[i] > value_)
        {
            current->dataArray[i+1] = current->dataArray[i];
            i--;
        }

        // put in the value and update the number of keys
        current->dataArray[i+1] = value_;
        current->keys++;
    }
    // if current is not a leaf
    else
    {
        // get the position at which the insert should be
        while (i >= 0 && current->dataArray[i] > value_)
            i--;

        // if the child in which we would like to put the new value is full, we need to split it and move the median value up to current
        if (current->children[i+1]->isFull())
        {
            // new index value for convenience
            int q = i + 1;

            // only create one new node this time because we already have the parent node
            BTNode<dataType>* sibling = new BTNode<dataType>(current->children[i + 1]->degree, current->children[i + 1]->leaf);


            // move the data from current to the new node
            for (int j = 0; j < degree-1; j++)
                sibling->dataArray[j] = current->children[i + 1]->dataArray[j + degree];
            if (!current->children[i + 1]->leaf)
            {
                for (int j = 0; j < degree; j++)
                    sibling->children[j] = current->children[i + 1]->children[j + degree];
            }


            // make space in current for the median node which is moving up
            for (int j = current->keys; j >= q+1; j--)
                current->children[j+1] = current->children[j];
            for (int j = current->keys - 1; j >= q; j--)
                current->dataArray[j+1] = current->dataArray[j];

            // put the node in place
            current->dataArray[q] = current->children[i + 1]->dataArray[degree - 1];

            // update pointers and keys
            current->children[i + 1]->keys = degree - 1;
            sibling->keys = degree - 1;
            current->keys++;
            current->children[q+1] = sibling;

            // see which child we would like to go into
            if (current->dataArray[i+1] < value_)
                i++;
        }
        // do it all again with that child!
        insertHelper(value_, current->children[i+1]);
    }
}

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
        if(Root->leaf)
            Root = NULL;
        else
            Root = Root->children[0];
        delete temp;
    }
}

//-------------------------------------------------
// Method: Print
// Purpose: Print the tree in sorted order
// Date: 08/01/18
// Author: Davis Campbell
//-------------------------------------------------
template <class dataType>
void BTree<dataType>::print() const
{
    cout << "In print" << endl;
    if (Root != NULL)
        printHelper(Root);
}

//-------------------------------------------------
// Method: Print Helper
// Purpose: print nodes recursively
// Date: 08/01/18
// Author: Davis Campbell
//-------------------------------------------------
template <class dataType>
void BTree<dataType>::printHelper(BTNode<dataType>* current) const
{
    if (current != NULL)
    {
        // loop through the node (remember we have current->keys keys and current->keys + 1 children)
        int i;
        for (i = 0; i < current->keys; i++)
        {
            if(!current->leaf)
                printHelper(current->children[i]);
            cout << current->dataArray[i] << " ";
        }
        if(!current->leaf)
            printHelper(current->children[i]);
    }
}
