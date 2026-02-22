#include "RequestQueue.h"
#include <new> // for std::nothrow

RequestQueue::RequestQueue()
    : data(nullptr),
      capacity(0),
      front(0),
      rear(0),
      count(0) {
    // start with a small default capacity
    resize(4);
}

RequestQueue::RequestQueue(int initialCapacity)
    : data(nullptr),
      capacity(0),
      front(0),
      rear(0),
      count(0) {
    if (initialCapacity < 1) {
        initialCapacity = 4;
    }
    resize(initialCapacity);
}

RequestQueue::~RequestQueue() {
    delete[] data;
}

bool RequestQueue::isEmpty() const {
    return count == 0;
}

bool RequestQueue::isFull() const {
    return count == capacity;
}

int RequestQueue::size() const {
    return count;
}

int RequestQueue::nextIndex(int idx) const {
    if (capacity == 0) return 0;
    return (idx + 1) % capacity;
}

bool RequestQueue::enqueue(const Request& req) {
    //Implement enqueue function as explained in the PDF.
    // Check for capacity and resize if needed
    if(isFull()){
        if(!resize(capacity * 2)){
            return false;
        }
    }
    // Enqueue the request at rear
    data[rear] = req;
    rear = nextIndex(rear);
    count++;
    return true;
}

bool RequestQueue::dequeue(Request& outReq) {
    //Implement dequeue function as explained in the PDF.
    // Check if queue is empty
    if(isEmpty()){
        return false;
    }
    // Dequeue the request at front
    outReq = data[front];
    front = nextIndex(front);
    count--;
    return true;
}

bool RequestQueue::peek(Request& outReq) const {
    //Implement peek function as explained in the PDF.
    // Check if queue is empty
    if(isEmpty()){
        return false;
    }
    // Assinging front request to outReq without removing
    outReq = data[front];
    return true;
}

void RequestQueue::clear() {
    front = 0;
    rear = 0;
    count = 0;
}

bool RequestQueue::removeById(const std::string& id) {
    //Implement removeById function as explained in the PDF.
    for(int i = 0; i < count; i++){
        int idx = (front + i) % capacity;
        if(data[idx].getId() == id){
            // Shift elements to fill the gap
            for(int j = i; j < count - 1; j++){
                int fromIdx = (front + j + 1) % capacity;
                int toIdx = (front + j) % capacity;
                data[toIdx] = data[fromIdx];
            }
            // Setting new rear position
            rear = (rear - 1 + capacity) % capacity;
            count--;
            return true;
        }
    }
    return false;
}

bool RequestQueue::resize(int newCapacity) {
    //Implement resize function as explained in the PDF.
    // Return false on allocation failure.
    if(newCapacity < count){
        return false;
    }
    // Allocate new array
    Request* newData = new (std::nothrow) Request[newCapacity];
    // Check for allocation failure
    if(newData == nullptr){
        return false;
    }
    for(int i = 0; i < count; i++){ // Transfer elements to new array
        newData[i] = data[(front + i) % capacity];
    }
    delete[] data; // Free old array
    // Update pointers and indices
    data = newData;
    capacity = newCapacity;
    front = 0;
    rear = count % capacity;
    return true;
}
