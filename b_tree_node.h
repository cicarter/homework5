#ifndef B_TREE_H
#define B_TREE_H

//-------------------------------------------------
// FileName: b_tree_node.h
// Purpose: Define and implement the methods and data associated with the BTNode class
// Date: 07/17/18
// Authors: Chris Carter, Davis Campbell, Wesley Nimmo, Sam Yeager
//-------------------------------------------------
//Using a template format for flexibility
template <class DataType>
class BTNode{
    public:
        //Constructors
        BTNode(int degree_, bool leaf_);

        // Deconstructor
        ~BTNode();

        //Methods
        bool isFull() const;
        void remove(DataType value_);

        template <class dataType>
        friend class BTree;

    private:
        int degree;
        int keys;
        bool leaf;
        DataType* dataArray;
        BTNode** children;
};

#include "b_tree_node.h"
#include <iostream>

using namespace std;

//-------------------------------------------------
// Method: Constructor
// Purpose: Initialize the private variables for the tree
// Date: 07/17/18
// Authors: Chris Carter, Davis Campbell, Wesley Nimmo, Sam Yeager
//-------------------------------------------------

template <class DataType>
BTNode<DataType>::BTNode(int degree_, bool leaf_)
{
    dataArray = new DataType[2 * degree_ - 1];
    children = new BTNode*[2 * degree_];
    degree = degree_;
    keys = 0;
    leaf = leaf_;
    for (int i = 0; i < 2 * degree_; i++)
        children[i] = NULL;
}

//-------------------------------------------------
// Method: Deconstructor
// Purpose: Free the memory associated with the BTNode
// Date: 07/17/18
// Author: Chris Carter
//-------------------------------------------------
template <class DataType>
BTNode<DataType>::~BTNode()
{
    delete[] dataArray;
    delete[] children;
}

//-------------------------------------------------
// Method: isFull
// Purpose: Return True if the node is full
// Date: 07/17/18
// Authors: Chris Carter, Davis Campbell
//-------------------------------------------------
template <class DataType>
bool BTNode<DataType>::isFull() const
{
    return keys == (2 * degree) - 1;
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

#endif
