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
    if (Root == NULL)
    {
        Root = new BTNode<dataType>(degree, true);
        Root->dataArray[0] = value_;
        Root->keys = 1;
    }
    else
    {
        if (!Root->isFull())
            insertHelper(value_, Root);
        else
        {
            BTNode<dataType>* newRoot = new BTNode<dataType>(degree, false);

            newRoot->children[0] = Root;

            BTNode<dataType>* sibling = new BTNode<dataType>(degree, newRoot->leaf);
            sibling->keys = degree - 1;

            for (int j = 0; j < degree - 1; j++)
                sibling->dataArray[j] = Root->dataArray[j+degree];

            if (!Root->leaf)
            {
                for (int j = 0; j < degree; j++)
                    sibling->children[j] = Root->children[j+degree];
            }

            Root->keys = degree - 1;

            for (int j = newRoot->keys; j >= 1; j--)
                newRoot->children[j+1] = newRoot->children[j];

            newRoot->children[1] = sibling;

            for (int j = newRoot->keys - 1; j >= 0; j--)
                newRoot->dataArray[j + 1] = newRoot->dataArray[j];

            newRoot->dataArray[0] = Root->dataArray[degree-1];

            newRoot->keys++;

            int i = 0;
            if (newRoot->dataArray[0] < value_)
                i++;
            insertHelper(value_, newRoot->children[i]);

            Root = newRoot;
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
    int i = current->keys - 1;

    if (current->leaf)
    {
        while (i >= 0 && current->dataArray[i] > value_)
        {
            current->dataArray[i+1] = current->dataArray[i];
            i--;
        }

        current->dataArray[i+1] = value_;
        current->keys++;
    }
    else
    {
        while (i >= 0 && current->dataArray[i] > value_)
            i--;

        if (current->children[i+1]->keys == 2*degree-1)
        {
            BTNode<dataType>* sibling = new BTNode<dataType>(current->children[i + 1]->degree, current->children[i + 1]->leaf);
            sibling->keys = degree - 1;

            for (int j = 0; j < degree-1; j++)
                sibling->dataArray[j] = current->children[i + 1]->dataArray[j+degree];

            if (current->children[i + 1]->leaf == false)
            {
                for (int j = 0; j < degree; j++)
                    sibling->children[j] = current->children[i + 1]->children[j+degree];
            }

            current->children[i + 1]->keys = degree - 1;

            for (int j = current->keys; j >= i+2; j--)
                current->children[j+1] = current->children[j];

            current->children[i+1] = sibling;

            for (int j = current->keys - 1; j >= i + 1; j--)
                current->dataArray[j+1] = current->dataArray[j];

            current->dataArray[i] = current->children[i + 1]->dataArray[degree-1];

            current->keys++;
            if (current->dataArray[i+1] < value_)
                i++;
        }
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
        if(Root->isLeaf())
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
