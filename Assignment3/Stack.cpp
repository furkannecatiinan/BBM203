#include "Stack.h"
#include <iostream>

Stack::Stack() {
    top = -1;
}

void Stack::push(int province) {
    if (top >= MAX_SIZE - 1){
        std::cerr << "Stackoverflow. Cannot push." << std::endl;
        return;
    }
    data[++top] = province;
}

int Stack::pop() {
    if(isEmpty()){
        std::cerr << "Stack underflow. Cannot pop."<<std::endl;
        return -1;
    }
    return data[top--];
}

int Stack::peek() const {
    if(isEmpty()){
        std::cerr << "Stack is empty. Cannot peek."<<std::endl;
        return -1;
    }
    return data[top];
}

bool Stack::isEmpty() const {
    return top == -1;
}

int Stack::getSize() const {
    return top + 1;
}


