#pragma once
#include <stdexcept>
#include "DynamicArray.h"

template <typename T, typename P>
class PriorityQueue {
private:
    struct Element {
        T item;
        P priority;
    };

    DynamicArray<Element> heap;

    void siftUp(int index) {
        while (index > 0) {
            int parentIndex = (index - 1) / 2;

            if (heap.get(index).priority < heap.get(parentIndex).priority) {
                heap.swap(index, parentIndex);
                index = parentIndex;
            } else {
                break;
            }
        }
    }

    void siftDown(int index) {
        int size = static_cast<int>(heap.getLength());
        while (true) {
            int leftChild = 2 * index + 1;
            int rightChild = 2 * index + 2;
            int smallest = index;

            if (leftChild < size && heap.get(leftChild).priority < heap.get(smallest).priority) {
                smallest = leftChild;
            }

            if (rightChild < size && heap.get(rightChild).priority < heap.get(smallest).priority) {
                smallest = rightChild;
            }

            if (smallest != index) {
                heap.swap(index, smallest);
                index = smallest; 
            } else {
                break; 
            }
        }
    }

public:
    PriorityQueue() {}

    bool isEmpty() const {
        return heap.getLength() == 0;
    }

    int count() const {
        return static_cast<int>(heap.getLength());
    }

    void enqueue(T item, P priority) {
        Element newEl = {item, priority};
        heap.append(newEl);
        siftUp(static_cast<int>(heap.getLength()) - 1);
    }

    T extractMin() {
        if (isEmpty()) {
            throw std::out_of_range("PriorityQueue is empty");
        }

        T result = heap.get(0).item;

        Element lastElement = heap.getLast();
        heap.set(0, lastElement);
        heap.removeLast();

        if (!isEmpty()) {
            siftDown(0);
        }

        return result;
    }
    
    T peek() {
        if (isEmpty()) throw std::out_of_range("PriorityQueue is empty");
        return heap.get(0).item;
    }
};
