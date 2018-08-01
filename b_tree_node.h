#ifndef B_TREE_H
#define B_TREE_H

/* 6hris' Notes

    While I tried using normal syntax for making the arrays (no DMA)
    Compilation failed. So now all the node memory is allocated to the heap
    Management of pointers and freeing memory is critical

*/

// I added the "keys" variable, which keeps track of the number of keys in each node.
// Make sure in your methods to only have/look for data in the array spots from 0 to keys - 1
// Without doing things like this I could not implement a valid remove function
// - Davis


/* B_Tree Node in a nutshell

    Each Node is an array of size degree - 1, where degree is now many
    children / branches a node could have

    The degree of a tree should not be changed, else the tree has
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
        BTNode(int degree_);

        // Copy constructor
        BTNode(const BTNode& from);

        // Deconstructor
        ~BTNode();

        //Methods
        bool isFull() const;
        bool isEmpty() const;
        bool isLeaf() const;
        void remove(DataType value_);

        // temporary methods (delete before submission
        void Print();
        void insertNonFull(int value_);
        void splitChild(int i, BTNode* y);


        template <class dataType>
        friend class BTree;

    private:
        int degree;
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
BTNode<DataType>::BTNode(int degree_)
{
    dataArray = new DataType[2 * degree_ - 1];
    children = new BTNode*[2 * degree_];
    degree = degree_;
    keys = 0;
    leaf = true;
    for (int i = 0; i < 2 * degree_; i++)
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

template <class DataType>
bool BTNode<DataType>::isEmpty() const
{
    return dataArraySize == 0;
}

template <class DataType>
bool BTNode<DataType>::isFull() const
{
    return dataArraySize == (2 * degree) - 1;
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

//-------------------------------------------------
// Method: remove
// Purpose: remove an element from the tree
// Date: 07/29/18
// Author: Davis Campbell
// Logic and general framework for algorithm from geeksforgeeks.org
//-------------------------------------------------
template <class DataType>
void BTNode<DataType>::remove(DataType value_)
{
    int i = 0;
    while(i < keys && dataArray[i] < value_)
         i++;

    if(dataArray[i] != value_ || i >= keys)
    {
        if(leaf)
        {
            cout << "The value " << value_ << " is not in this tree.\n";
            return;
        }

        bool keyPresent = (i == keys? true : false);

        // We have to make sure the child has enough keys for when we delete a key
        if (children[i]->keys < degree)
        {
            // From the logic, if the next child node has more than degree-1 keys we take a key from that child
            if (i != keys && children[i + 1]->keys > degree - 1)
            {
                BTNode* First = children[i];
                BTNode* Next = children[i + 1];

                // copy the value at the i to the end of the First node
                First->dataArray[First->keys] = dataArray[i];

                // copy the first child from the Next node to the end of the first node
                if(!First->leaf)
                    First->children[First->keys + 1] = Next->children[0];

                // copy the first value of the Next node to position i
                dataArray[i] = Next->dataArray[0];

                // Move all the values back in the Next node
                for (int j = 0; j < Next->keys; j++)
                    Next->dataArray[j] = Next->dataArray[j + 1];

                // move all the children pointers back in the Next node if it is not a leaf
                if (!Next->leaf)
                    for (int k = 0; k < Next->keys; k++)
                        Next->children[k] = Next->children[k + 1];

                // change the number of keys in each changed node
                Next->keys = Next->keys - 1;
                First->keys = First->keys + 1;
            }

            // From the logic, if the previous child node has more than degree - 1 keys, take a key from that child
            else if (i != 0 && children[i - 1]->keys > degree - 1)
            {
                BTNode* First = children[i];
                BTNode* Previous = children[i - 1];

                // move all the data in First to the right
                for (int j = First->keys - 1; j > -1; j--)
                    First->dataArray[j+1] = First->dataArray[j];

                //move all the children over as well
                if (!First->leaf)
                    for(int j = First->keys; j > -1; j--)
                        First->children[j+1] = First->children[j];

                // move the value at i to the first spot in First
                First->dataArray[0] = dataArray[i-1];

                // move the last value in previous to the spot at i
                dataArray[i] = Previous->dataArray[Previous->keys];

                // move the previous child over
                if (!First->leaf)
                    First->children[0] = Previous->children[Previous->keys];

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
                    BTNode* First = children[i];
                    BTNode* Next = children[i + 1];

                    // copy value at current position to the end of first
                    First->dataArray[degree - 1] = dataArray[i];

                    // copy child pointers over from the next node to the end of the first node
                    if (!First->leaf)
                    {
                        for(int j = 0; j <= Next->keys; j++)
                            First->children[degree + j] = Next->children[j];
                    }

                    // copy the values from next to first
                    for(int j = 0; j < Next->keys; j++)
                        First->dataArray[degree + j] = Next->dataArray[j];

                    // fill in the gap in the current node
                    for (int j = i + 2; j <= keys; j++)
                        children[j - 1] = children[j];
                    for (int j = i + 1; j < keys; j++)
                        dataArray[j - 1] = dataArray[j];

                    // update key counts and free memory
                    keys -= 1;
                    First->keys = First->keys + Next->keys + 1;
                    delete Next;
                }
                else
                {
                    // merge with previous sibling
                    BTNode* Previous = children[i - 1];
                    BTNode* First = children[i];

                    // copy value at current position to the end of first
                    Previous->dataArray[degree - 1] = dataArray[i-1];

                    // copy child pointers over from the next node to the end of the first node
                    if (!First->leaf)
                    {
                        for(int j = 0; j <= First->keys; j++)
                            Previous->children[degree + j] = First->children[j];
                    }

                    // copy the values from next to first
                    for(int j = 0; j < First->keys; j++)
                        Previous->dataArray[degree + j] = First->dataArray[j];

                    // fill in the gap in the current node
                    for (int j = i + 1; j <= keys; j++)
                        children[j - 1] = children[j];
                    for (int j = i; j < keys; j++)
                        dataArray[j - 1] = dataArray[j];

                    // update key counts and free memory
                    keys -= 1;
                    Previous->keys = Previous->keys + First->keys + 1;
                    delete First;
                }
            }
        }

        if (keyPresent && i > keys)
            children[i - 1]->remove(value_);
        else
            children[i]->remove(value_);
    }
    else if(dataArray[i] == value_)
    {
        if (!leaf)
        {
            // by the logic, if the child at i has at least as many keys as the degree, we delete the predecessor of the key to be deleted and then put it in the key to be deleted's space
            if (children[i]->keys >= degree)
            {
                BTNode* Predecessor = children[i];
                while(!Predecessor->leaf)
                    Predecessor = Predecessor->children[Predecessor->keys];
                DataType Prior = Predecessor->dataArray[Predecessor->keys - 1];
                dataArray[i] = Prior;
                children[i]->remove(Prior);
            }
            // basically the opposite of above
            else if (children[i + 1]->keys >= degree)
            {
                BTNode* Successor = children[i + 1];
                while (!Successor->leaf)
                    Successor = Successor->children[0];
                DataType Post = Successor->dataArray[0];
                dataArray[i] = Post;
                children[i + 1]->remove(Post);
            }
            // if both the child and the predecessor dont have enough, merge the child into the predecessor along with the value, delete the value and the child.
            else
            {
                // merge with next sibling
                BTNode* First = children[i];
                BTNode* Next = children[i + 1];

                // copy value at current position to the end of first
                First->dataArray[degree - 1] = dataArray[i];

                // copy child pointers over from the next node to the end of the first node
                if (!First->leaf)
                {
                    for(int j = 0; j <= Next->keys; j++)
                        First->children[degree + j] = Next->children[j];
                }

                // copy the values from next to first
                for(int j = 0; j < Next->keys; j++)
                    First->dataArray[degree + j] = Next->dataArray[j];

                // fill in the gap in the current node
                for (int j = i + 2; j <= keys; j++)
                    children[j - 1] = children[j];
                for (int j = i + 1; j < keys; j++)
                    dataArray[j - 1] = dataArray[j];

                // update key counts and free memory
                keys -= 1;
                First->keys = First->keys + Next->keys + 1;
                delete Next;

                children[i]->remove(value_);
            }
        }
        else
        {
            // move keys backward and reduce count
            for (int j = i + 1; j < keys; j++)
                dataArray[j - 1] = dataArray[j];
            keys--;
        }
    }
}








/*
From here on is just a temporary insert implementation from the Internet so i can test remove
Don't forget to delete because this was straight up copied lol
*/

// A utility function to insert a new key in this node
// The assumption is, the node must be non-full when this
// function is called
template <class DataType>
void BTNode<DataType>::insertNonFull(int value_)
{
    // Initialize index as index of rightmost element
    int i = keys-1;

    // If this is a leaf node
    if (leaf == true)
    {
        // The following loop does two things
        // a) Finds the location of new key to be inserted
        // b) Moves all greater keys to one place ahead
        while (i >= 0 && dataArray[i] > value_)
        {
            dataArray[i+1] = dataArray[i];
            i--;
        }

        // Insert the new key at found location
        dataArray[i+1] = value_;
        keys = keys+1;
    }
    else // If this node is not leaf
    {
        // Find the child which is going to have the new key
        while (i >= 0 && dataArray[i] > value_)
            i--;

        // See if the found child is full
        if (children[i+1]->keys == 2*degree-1)
        {
            // If the child is full, then split it
            splitChild(i+1, children[i+1]);

            // After split, the middle key of C[i] goes up and
            // C[i] is splitted into two.  See which of the two
            // is going to have the new key
            if (dataArray[i+1] < value_)
                i++;
        }
        children[i+1]->insertNonFull(value_);
    }
}

// A utility function to split the child y of this node
// Note that y must be full when this function is called
template <class DataType>
void BTNode<DataType>::splitChild(int i, BTNode *y)
{
    // Create a new node which is going to store (t-1) keys
    // of y
    BTNode *z = new BTNode(y->degree, y->leaf);
    z->keys = degree - 1;

    // Copy the last (t-1) keys of y to z
    for (int j = 0; j < degree-1; j++)
        z->dataArray[j] = y->dataArray[j+degree];

    // Copy the last t children of y to z
    if (y->leaf == false)
    {
        for (int j = 0; j < degree; j++)
            z->children[j] = y->children[j+degree];
    }

    // Reduce the number of keys in y
    y->keys = degree - 1;

    // Since this node is going to have a new child,
    // create space of new child
    for (int j = keys; j >= i+1; j--)
        children[j+1] = children[j];

    // Link the new child to this node
    children[i+1] = z;

    // A key of y will move to this node. Find location of
    // new key and move all greater keys one space ahead
    for (int j = keys-1; j >= i; j--)
        dataArray[j+1] = dataArray[j];

    // Copy the middle key of y to this node
    dataArray[i] = y->dataArray[degree-1];

    // Increment count of keys in this node
    keys = keys + 1;
}

// Function to Print all nodes in a subtree Rooted with this node
template <class DataType>
void BTNode<DataType>::Print()
{
    // There are n keys and n+1 children, travers through n keys
    // and first n children
    int i;
    for (i = 0; i < keys; i++)
    {
        // If this is not leaf, then before printing key[i],
        // Print the subtree Rooted with child C[i].
        if (leaf == false)
            children[i]->Print();
        cout << " " << dataArray[i];
    }

    // Print the subtree Rooted with last child
    if (leaf == false)
        children[i]->Print();
}

#endif
