#ifndef B_TREE_H
#define B_TREE_H

/* Chris' Notes

    While I tried using normal syntax for making the arrays (no DMA)
    Compilation failed. So now all the node memory is allocated to the heap
    Management of pointers and freeing memory is critical

*/

/* B_Tree Node in a nutshell

    Each Node is an array of size M - 1, where M is now many 
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
        BTNode(int M = 4);
        BTNode(const BTNode& from);
        ~BTNode();

        //Getters
        DataType* getDataArray() const;
        BTNode** getChildren() const;

        //Setters
        void setDataArray(DataType* arr);
        void setChildren(BTNode** childen);

        //Methods
        bool IsFull() const;
        bool isEmpty() const;

    private:
        int M = 4;
        DataType* dataArray;
        int dataArraySize;
        BTNode** children;
        int childrenArraySize;


};

#endif
