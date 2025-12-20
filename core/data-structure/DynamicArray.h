#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdexcept>
#include <utility>
#include "CustomErrors.h"

template<typename T>
class DynamicArray {
private:
    T* data;
    size_t size;
public:
    DynamicArray() : data(nullptr), size(0) {}
    DynamicArray(T* items, int count);
    DynamicArray(int size);
    DynamicArray(const DynamicArray<T>& dynamicArray);
    ~DynamicArray();
    
    T& get(int index) const;
    T* getData() const;
    void set(int index, T value);
    void append(T value);
    void swap(int first, int second);
    T& getLast() const;
    void removeLast();
    size_t getSize() const;
    size_t getLength() const;
    void resize(int newSize, const T& el = T());
    void removeAt(int index);
    T* getPointer(int index);

};

template<typename T>
DynamicArray<T>::DynamicArray(T* items, int count) {
    if (count < 0) {
        throw NotValidArgument;
    }
    if (!items && count > 0) {
        throw NullPointerPassedAsArgument;
    }
    size = count;
    data = new T[size]{};
    for (int i = 0; i < count; ++i) {
        data[i] = items[i];
    }
}

template<typename T>
DynamicArray<T>::DynamicArray(int size) {
    if (size < 0) {
        throw IndexOutOfRange;
    }
    data = new T[size]{};
    this->size = size;
}

template<typename T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& dynamicArray) {
    size = dynamicArray.size;
    data = new T[size]{};
    for (int i = 0; i < size; ++i) {
        data[i] = dynamicArray.data[i];
    }
}

template<typename T>
DynamicArray<T>::~DynamicArray() {
    delete[] data;
}

template<typename T>
T& DynamicArray<T>::get(int index) const {
    if (index < 0 || index >= size) {
        throw IndexOutOfRange;
    }
    return data[index];
}

template<typename T>
void DynamicArray<T>::removeAt(int index){
    if( index < 0 || index >= size){
        throw IndexOutOfRange;
    }
    T* newData = new T[size-1]{};
    for (int i = 0; i < index; i++){
        newData[i] = data[i];
    }
    for (int i = index; i < size-1; ++i){
        newData[i] = data[i+1];
    }
    delete[] data;
    data = newData;
    size--;
}

template<typename T>
T* DynamicArray<T>::getData() const {
    return this->data;
}

template<typename T>
void DynamicArray<T>::set(int index, T value) {
    if (index < 0 || index > size) {
        throw IndexOutOfRange;
    }
    if (index == size){
        this->resize(size + 1);
    }
    data[index] = value;
}

template<typename T>
void DynamicArray<T>::append(T value){
    this->resize(size + 1);
    data[size-1] = value;
}


template<typename T>
size_t DynamicArray<T>::getSize() const {
    return size;
}

template<typename T>
size_t DynamicArray<T>::getLength() const {
    return size;
}

template<typename T>
T* getPointer(int index) {
        if (index < 0 || index >= getLength()) throw std::out_of_range("Index out of bounds");
        return &data[index]; 
    }

template<typename T>
T& DynamicArray<T>::getLast() const {
    if (size == 0) {
        throw IndexOutOfRange;
    }
    return data[size - 1];
}

template<typename T>
void DynamicArray<T>::removeLast() {
    if (size == 0) {
        throw IndexOutOfRange;
    }
    resize(static_cast<int>(size - 1));
}

template<typename T>
void DynamicArray<T>::swap(int first, int second) {
    if (first < 0 || second < 0 || first >= size || second >= size) {
        throw IndexOutOfRange;
    }
    std::swap(data[first], data[second]);
}

template<typename T>
void DynamicArray<T>::resize(int newSize, const T& el) { 
    if (newSize < 0) {
        throw NotValidArgument;
    }
    T* newData = new T[newSize]{};
    if (newSize < size) {
        for (int i = 0; i < newSize; ++i) {
            newData[i] = data[i];
        }
    }
    else {
        for (int i = 0; i < size; ++i) {
            newData[i] = data[i];
        }
        for (int i = size; i < newSize; ++i){
            newData[i] = el;
        }
    }
    delete[] data;
    data = newData;
    size = newSize;
}

#endif
