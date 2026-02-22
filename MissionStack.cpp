#include "MissionStack.h"
#include <new>     // for std::nothrow

MissionStack::MissionStack()
    : data(nullptr),
      capacity(0),
      top(-1) {
    resize(4);
}

MissionStack::MissionStack(int initialCapacity)
    : data(nullptr),
      capacity(0),
      top(-1) {
    if (initialCapacity < 1) {
        initialCapacity = 4;
    }
    resize(initialCapacity);
}

MissionStack::~MissionStack() {
    delete[] data;
}

bool MissionStack::isEmpty() const {
    return top == -1;
}

int MissionStack::size() const {
    return top + 1;
}

bool MissionStack::push(const Request& req) {
    //Implement push function as explained in the PDF.
    // Check for capacity and resize if needed
    if(size() == capacity){
        if(!resize(capacity * 2)){
            return false;
        }
    }
    // Pushing the request onto the stack
    data[top + 1] = req;
    top++;
    
    return true;
}

bool MissionStack::pop(Request& outReq) {
    //Implement pop function as explained in the PDF.
    // Check if stack is empty
    if(isEmpty()){
        return false;
    }
    // Pop the top request
    outReq = data[top];
    top--;
    return true;
}

bool MissionStack::peek(Request& outReq) const {
    //Implement peek function as explained in the PDF.
    // Check if stack is empty
    if(isEmpty()){
        return false;
    }
    // Peek the top request
    outReq = data[top];
    return true;
}

void MissionStack::clear() {
    top = -1;
}

bool MissionStack::resize(int newCapacity) {
    //Implement resize function as explained in the PDF.
    // Check if new capacity is bigger than current size
    if (newCapacity < size()) {
        return false;
    }
    Request* newData = new (std::nothrow) Request[newCapacity];
    // Check for allocation failure
    if (newData == nullptr) {
        return false;
    }
    // Copy existing elements to new array
    for (int i = 0; i <= top; i++) {
        newData[i] = data[i];
    }
    // Delete old array and update pointers
    delete[] data;
    data = newData;
    capacity = newCapacity;
    return true;
}
