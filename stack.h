//
// Created by ILHAN on 23.10.2023.
//

#ifndef CS300_THE1_STACK_H
#define CS300_THE1_STACK_H

template <class Object>
class stack {
public:
    stack();
    stack(stack<Object> & rhs);
    ~stack();
    void pop();
    void emptyStack();
    bool isEmpty();
    void push(const Object & cell);
    Object getTop() const;
    Object topAndPop();
    int getSize() const;
    void changeObj(Object & obj);
    //bool contains(const Object & elem);
    stack<Object> & operator=(const stack & rhs);
private:
    struct Node {
        Object element;
        Node * next;

        Node(const Object & elem1, Node * ptr = NULL) //Sus
                :element(elem1), next(ptr) {}
    };
    Node * top;
    int size;
};

#include "stack.cpp"
#endif //CS300_THE1_STACK_H
