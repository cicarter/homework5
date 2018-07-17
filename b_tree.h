#include "b_tree_node.h"
#include <cstdlib>

template <class dataType>
class BTree{

    public:
        BTree(int order);
        BTree(const BTree& obj);
        ~BTree();

        //Getters
        int getOrder() const;
        BTNode<dataType>* getRoot() const;

        //Setters
        void setRoot(BTNode<dataType>* Root);

        //Common Data Structures Methods
        BTNode<dataType>* search(dataType val);
        void insert(dataType val);
        bool remove(dataType val);
        void print() const;

    private:
        //Member Variables
        int order;
        BTNode<dataType>* Root;

        //Helper Functinos
        BTNode<dataType>* searchHelper(dataType val, BTNode<dataType>* curr);

};

