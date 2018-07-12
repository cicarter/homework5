
#ifndef B_TREE_H
#define B_TREE_H

template <class DataType>
class BTNode{
    public:
        BTNode(int M = 4);
        ~BTNode();

        //Getters
        int getOrder() const;
        DataType* getDataArray() const;
        BTNode** getChildren() const;

        //Setters
        void setDataArray(const DataType* arr);
        void setChildren(const BTNode** childen);

    private:
        DataType* dataArray;
        int order;
        BTNode** children;


};

#endif
