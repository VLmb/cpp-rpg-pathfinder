#pragma once
#include <stdexcept>
#include <functional>
#include <utility>
#include "DynamicArray.h"

template <typename T>
class PriorityQueue {
private:
    DynamicArray<T> heap;


    std::function<bool(const T&, const T&)> comparator;

private:
    bool less(const T& a, const T& b) const {
        if (comparator) return comparator(a, b);
        return a < b;
    }

    void siftUp(int index) {
        while (index > 0) {
            int parentIndex = (index - 1) / 2;

            if (less(heap.get(index), heap.get(parentIndex))) {
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
            int leftChild  = 2 * index + 1;
            int rightChild = 2 * index + 2;
            int smallest = index;

            if (leftChild < size && less(heap.get(leftChild), heap.get(smallest))) {
                smallest = leftChild;
            }
            if (rightChild < size && less(heap.get(rightChild), heap.get(smallest))) {
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
    PriorityQueue() = default;

    explicit PriorityQueue(std::function<bool(const T&, const T&)> comparator)
        : comparator(std::move(comparator)) {}

    bool isEmpty() const {
        return heap.getLength() == 0;
    }

    int count() const {
        return static_cast<int>(heap.getLength());
    }

    void enqueue(const T& item) {
        heap.append(item);
        siftUp(static_cast<int>(heap.getLength()) - 1);
    }

    void enqueue(T&& item) {
        heap.append(std::move(item));
        siftUp(static_cast<int>(heap.getLength()) - 1);
    }

    T extractMin() {
        if (isEmpty()) throw std::out_of_range("PriorityQueue is empty");

        T result = heap.get(0);

        T lastElement = heap.getLast();
        heap.set(0, std::move(lastElement));
        heap.removeLast();

        if (!isEmpty()) siftDown(0);

        return result;
    }

    const T& peek() const {
        if (isEmpty()) throw std::out_of_range("PriorityQueue is empty");
        return heap.get(0);
    }
};
