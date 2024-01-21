//
// Created by ILHAN on 23.10.2023.
//

#include <iostream>
#include "stack.h"

// Default constructor
template <class Object>
stack<Object>::stack() {
    top = nullptr;
    size = 0;
}

// Deep copy constructor (Taken from the slides)
template <class Object>
stack<Object>::stack(stack<Object> & rhs) {
    top = nullptr;
    *this = rhs;
    size = rhs.size;
}

// Destructor (Taken from the slides)
template <class Object>
stack<Object>::~stack() {
    emptyStack();
}

// Taken from the slides
template <class Object>
void stack<Object>::emptyStack() {
    while (!isEmpty())
        pop();
}

// Taken from the slides
template <class Object>
bool stack<Object>::isEmpty() {
    if (top == nullptr)
        return true;
    else
        return false;
}

// Taken from the slides
template <class Object>
void stack<Object>::pop() {
    if (!isEmpty()) {
        Node * ptr = top;
        top = top->next;
        delete ptr;
        ptr = nullptr;
        size--;
    }
}

// Taken from the slides
template <class Object>
void stack<Object>::push(const Object & cell) {
    top = new Node(cell, top);
    size++;
}


template <class Object>
Object stack<Object>::getTop() const { //Problem
    return top->element;

}

template <class Object>
int stack<Object>::getSize() const{
    return size;
}

// Taken from the slides
template <class Object>
Object stack<Object>::topAndPop()
{
    Object topItem = this->getTop();
    pop();
    return topItem;
}

template <class Object>
void stack<Object>::changeObj(Object &obj) {
    top->element = obj;
}

template <class Object>
stack<Object> & stack<Object>::operator =(const stack & rhs) {
    stack<Object> myStack;
    Node * ptr = rhs.top; // A pointer that points the top of the right hand side stack
    this->emptyStack(); // Emptying the left hand side stack
    while (ptr != nullptr) {
        myStack.push(ptr->element);
        ptr = ptr->next;
    }
    ptr = myStack.top;
    while (ptr != nullptr) {
        this->push(ptr->element);
        ptr = ptr->next;
    }

    return *this;
}