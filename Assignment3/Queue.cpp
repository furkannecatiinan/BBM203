#include "Queue.h"
#include <iostream>


Queue::Queue() {
    front = -1;
    rear = -1;
}


void Queue::enqueue(int province) {
    if((rear + 1) % MAX_QUEUE_SIZE == front){
    std::cerr << "Queue overflow. Cannot enqueue." << std::endl;
    return;
    }
    if (isEmpty()){
        front = 0;
        rear = 0;
    } else{
        rear = (rear + 1) % MAX_QUEUE_SIZE;
    }
    data[rear] = province;

}


int Queue::dequeue() {
    if (isEmpty()){
        std::cerr << "Queue undeflow. Cannot dequeue. " << std::endl;
        return -1;
    }
    int province = data[front];
    if(front == rear){
        front = -1;
        rear = -1;
    } else {
        front = (front + 1) % MAX_QUEUE_SIZE;
    }
    return province;
}

int Queue::peek() const {
    if(isEmpty()){
        std::cerr << "Queue is empty. Cannot peek." << std::endl;
    }
    return data[front];
}

bool Queue::isEmpty() const {
    return front == -1;
}

void Queue::enqueuePriority(int province) {
    if ((rear + 1) % MAX_QUEUE_SIZE == front) {
        std::cerr << "Queue overflow. Cannot enqueue priority." << std::endl;
        return;
    }

    if (isEmpty()) {
        enqueue(province);
        return;
    }

    front = (front - 1 + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;
    data[front] = province;
}

