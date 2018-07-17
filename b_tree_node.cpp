#include "b_tree_node.h"
#include <iostream>

using namespace std;

template <class DataType>
BTNode<DataType>::BTNode(int M = 4)
{
    dataArray = new DataType[M];
    children = new BTNode*[M];
    order = M;
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
DataType* BTNode<DataType>::getDataArray() const
{
    return dataArray;
}

template <class DataType>
BTNode<DataType>** BTNode<DataType>::getChildren() const
{
    return children;
}

template <class DataType>
void BTNode<DataType>::setDataArray(DataType* arr)
{
    for(int i = 0; i < order - 1; i++)
        dataArray[i] = arr[i];
}

template <class DataType>
void BTNode<DataType>::setChildren(BTNode** Children)
{
    children = Children;
}


int main()
{
    BTNode<int> test(4);

    //Setting the array

    const int size = 3;
    int array[size];

    for(int i = 0; i < size; i++)
        array[i] = i + 1;

    test.setDataArray(array);

    int* print;
    print = test.getDataArray();

    for(int i = 0; i < 3; i++)
        cout << print[i] << " -> ";

    cout << endl;

}

template <class DataType>
bool BTNode<DataType>::isEmpty() const
{
    if(dataArraySize == 0)
        return true;
    else
        return false;
}

template <class DataType>
bool BTNode<DataType>::isFull() const
{
    if(dataArraySize == order - 1)
        return true;
    else
        return false;
}
