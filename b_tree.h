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
        void insertTemp(dataType);
        void remove(dataType value_);
        void print();

        //Temporary methods (delete later)
        void Print()
        {
            if (Root != NULL) Root->Print();
        }
    private:

        //Member Variables
        int degree;
        BTNode<dataType>* Root;

        //Helper Functinos
        BTNode<dataType>* searchHelper(dataType val, BTNode<dataType>* curr);
        BTNode<dataType>* merge(BTNode<dataType>*, BTNode<dataType>*);
        BTNode<dataType>* breakNode(BTNode<dataType>*&, BTNode<dataType>*);
        BTNode<dataType>* insertHelper(dataType, BTNode<dataType>*&);
        void printHelper(BTNode<dataType>* node);
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

template <class dataType>
void BTree<dataType>::insert(dataType val)
{
    insertHelper(val, Root);
}

template <class dataType>
BTNode<dataType>* BTree<dataType>::merge(BTNode<dataType>* main, BTNode<dataType>* side)
{
    if(!main -> isFull())
    {
        int i;
        for(i = main -> dataArraySize; main -> children[i] != side; i--)
        {
            main -> dataArray[i] = main -> dataArray[i -1];
            main -> children[i] = main -> children[i - 1];
        }
        main -> dataArray[i] = side -> dataArray[0];
        main -> children[i] = side -> children[0];
        main -> children[i + 1] = side -> children[1];
        main -> dataArraySize++;
        main -> leaf = main -> isLeaf();
        return NULL;

    }
    else
        return breakNode(main, side);
}

//Put this in the BTNode Class
template <class dataType>
BTNode<dataType>* BTree<dataType>::breakNode(BTNode<dataType>*& node, BTNode<dataType>* single)
{

    //Put all the nodes into a bigger array
    BTNode<dataType>* together = new BTNode<dataType>(degree * 2 + 1);

    bool inserted = false;
    for(int i = 0, k = 0, c = 0; k <  degree * 2; i++, k++, c++)
    {
        if(k == degree * 2 - 1 && !inserted)
        {
            together -> dataArray[k] = single -> dataArray[0];
            together -> dataArraySize++;
            together -> children[c++] = single -> children[0];
            together -> children[c] = single -> children[1];
            inserted = true;
            break;
        }

        else if(single -> dataArray[0] < node -> dataArray[i] && !inserted)
        {
            together -> dataArray[k] = single -> dataArray[0];
            together -> dataArraySize++;
            together -> children[c++] = single -> children[0];
            together -> children[c] = single -> children[1];
            inserted = true;
        }

        together -> dataArray[k] = node -> dataArray[i];
        together -> dataArraySize++;
        together -> children[k] = node -> children[k];
    }

    //Calculate the median
    int median = (degree * 2 + 1) / 2 - 1;
    dataType val = together -> dataArray[median];

    //Everything in the array < med goes in split one
    BTNode<dataType>* split1 = new BTNode<dataType>(degree * 2);
    //Everything in the array > med goes in split one
    BTNode<dataType>* split2 = new BTNode<dataType>(degree * 2);

    //Set the data array
    for(int i = 0; i < median; i++)
        split1 -> dataArray[split1 -> dataArraySize++] = together -> dataArray[i];
    for(int i = median + 1; i < degree * 2; i++)
    {
        split2 -> dataArray[i - median - 1] = together -> dataArray[i];
        split2 -> dataArraySize++;
    }

    //Set up the children array
    for(int i = 0; i < (degree * 2 + 1) / 2; i++)
        split1 -> children[i] = together -> children[i];
    for(int i = (degree * 2 + 1) / 2; i < degree * 2 + 1; i++)
        split2 -> children[i - (degree * 2 + 1) / 2] = together -> children[i];

    //Set up node
    node -> dataArray[0] = val; //Problem Line
    node -> children[0] = split1;
    node -> children[1] = split2;
    node -> dataArraySize = 1;

    //Deleting tmp array
    delete together;

    //Return
    node -> leaf = node -> isLeaf();
    return node;

}

template <class dataType>
BTNode<dataType>* BTree<dataType>::insertHelper(dataType val, BTNode<dataType>*& curr)
{
    if(curr == NULL)
    {
        //Create a new node
        curr = new BTNode<dataType>(degree * 2);
        curr -> dataArray[0] = val;
        curr -> dataArraySize++;
        curr -> leaf = curr -> isLeaf();
        return NULL;
    }

    dataType* data = curr -> dataArray;
    BTNode<dataType>** children = curr -> children;

    if(curr -> isLeaf() && curr -> isFull())
    {
        BTNode<dataType>* valNode = new BTNode<dataType>(degree * 2);
        valNode -> dataArray[0] = val;
        return breakNode(curr, valNode);
    }
    else if(curr -> isLeaf())
    {
        int& offset = curr -> dataArraySize;
        data[offset++] = val;
        return NULL;
    }

    else
    {
        //Node must have child insert into child
        int i;
        for(i = 0; i < curr -> dataArraySize; i++)
            if(val < data[i])
                break;
        BTNode<dataType>* test = insertHelper(val, children[i]);
        if(test != NULL)
            return merge(curr, test);
        else
            return NULL;
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
// Purpose: Public print function that calls PrintHelper
// Date: 07/30/18
// Author: Sam Yeager
//-------------------------------------------------
template <class dataType>
void BTree<dataType>::print()
{
    printHelper(Root);
}

//-------------------------------------------------
// Method: printHelper
// Purpose: Private function that prints values in descending order (lowest value first).
// Date: 07/30/18
// Author: Sam Yeager
//-------------------------------------------------
template<class dataType>
void BTree<dataType>::printHelper(BTNode<dataType>* node)
{
    //TODO recursive call
}


/*
From here on is from the Internet just to test
*/

// The main function that inserts a new key in this B-Tree
template <class dataType>
void BTree<dataType>::insertTemp(dataType value_)
{
    // If tree is empty
    if (Root == NULL)
    {
        // Allocate memory for Root
        Root = new BTNode<dataType>(degree);
        Root->dataArray[0] = value_;  // Insert key
        Root->keys = 1;  // Update number of keys in Root
    }
    else // If tree is not empty
    {
        // If Root is full, then tree grows in height
        if (Root->keys == 2*degree-1)
        {
            // Allocate memory for new Root
            BTNode<dataType> *s = new BTNode<dataType>(degree);

            // Make old Root as child of new Root
            s->children[0] = Root;

            // Split the old Root and move 1 key to the new Root
            s->splitChild(0, Root);

            // New Root has two children now.  Decide which of the
            // two children is going to have new key
            int i = 0;
            if (s->dataArray[0] < value_)
                i++;
            s->children[i]->insertNonFull(value_);

            // Change Root
            Root = s;
        }
        else  // If Root is not full, call insertNonFull for Root
            Root->insertNonFull(value_);
    }
}



