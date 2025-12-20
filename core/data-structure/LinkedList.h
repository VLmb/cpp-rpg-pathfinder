#ifndef LINKED_LIST_ARRAY_H
#define LINKED_LIST_ARRAY_H

#include <stdexcept>


template<typename T>
class LinkedList{
protected:
    struct Node{
        T data;
        Node* next;
        Node(const T& data) : data(data), next(nullptr) {}
        Node(const T& data, Node* next) : data(data), next(next) {}
    };
    Node* head;
    Node* tail;
    size_t size;
public:
    LinkedList();
    LinkedList(T* items, int count);
    LinkedList(const LinkedList<T>* list);
    ~LinkedList();

    T getFirst() const;
    T getLast() const;
    T& get(int index) const;
    LinkedList<T>* getSubList(int startIndex, int endIndex) const;
    int getLength() const;
    bool equal(LinkedList<T>& otherList) const;

    void append(T item);
    void prepend(T item);
    void insertAt(T item, int index);
    void removeAt(int index);
    LinkedList<T>* concat(LinkedList<T>* list);
};

template<typename T>
LinkedList<T>::LinkedList() : head(nullptr), tail(nullptr), size(0) {}


template<typename T>
LinkedList<T>::LinkedList(T* items, int count) : head(nullptr), tail(nullptr), size(0) {
    if (items == nullptr && count > 0){
        throw NullPointerPassedAsArgument;
    }
    if (count < 0){
        throw NotValidArgument;
    }
    for (int i = 0; i < count; ++i){
        append(items[i]);
    }
}

template<typename T>
LinkedList<T>::LinkedList(const LinkedList<T>* list) : head(nullptr), tail(nullptr), size(0) {
    if (!list){
        throw NullPointerPassedAsArgument;
    }
    Node* cur = list->head;
    while (cur){
        append(cur->data);
        cur = cur->next;
    }
}

template<typename T>
T LinkedList<T>::getFirst() const {
    if (!head){
        throw EmptyContainer;
    }
    return head->data;
}

template<typename T>
T LinkedList<T>::getLast() const {
    if (!tail){
        throw EmptyContainer;
    }
    return tail->data;
}

template<typename T>
T& LinkedList<T>::get(int index) const {
    if (index < 0 || index >= size){
        throw IndexOutOfRange;
    }
    Node* cur = head;
    for (int i = 0; i < index; ++i){
        cur = cur->next;
    }
    return cur->data;
}

template<typename T>
LinkedList<T>* LinkedList<T>::getSubList(int startIndex, int endIndex) const {
    if (startIndex < 0 || endIndex >= size || startIndex > endIndex) {
        throw IndexOutOfRange;
    }
    LinkedList<T>* sublist = new LinkedList<T>();
    Node* cur = head;
    for (int i = 0; i < startIndex; ++i) {
        cur = cur->next;
    }
    for (int i = startIndex; i <= endIndex; ++i) {
        sublist->append(cur->data);
        cur = cur->next;
    }
    return sublist;
}

template<typename T>
int LinkedList<T>::getLength() const {
    return size;
}

template<typename T>
void LinkedList<T>::append(T item) {
    Node* newNode = new Node{item};
    if (!head){
        this->head = this->tail = newNode;
    }
    else{
        this->tail->next = newNode;
        this->tail = newNode;
    }
    ++size;

}

template<typename T>
void LinkedList<T>::removeAt(int index){
    if( index < 0 || index >= size){
        throw IndexOutOfRange;
    }
    Node* parent = nullptr;
    Node* cur = head;
    if (index == 0){
        head = cur->next;
    }
    else{
        for (int i = 0; i < index; ++i){
            parent = cur;
            cur = cur->next;
        }
        parent->next = cur->next;
    }
    delete cur;
    size--;

    if (size == 0) {
        head = nullptr;
        tail = nullptr;
    }
}

template<typename T>
void LinkedList<T>::prepend(T item){
    Node* newNode = new Node{item, head};
    head = newNode;
    if (!tail){
        tail = newNode;
    }
    ++size;
}

template<typename T>
void LinkedList<T>::insertAt(T item, int index){
    if (index < 0 || index > size){
        throw IndexOutOfRange;
    }
    if (index == 0){
        prepend(item);
        return;
    }
    if (index == size){
        append(item);
        return;
    }
    Node* cur = head;
    for (int i = 0; i < index - 1; ++i){
        cur = cur->next;
    }
    cur->next = new Node{item, cur->next};
    ++size;
}

template<typename T>
LinkedList<T>* LinkedList<T>::concat(LinkedList<T>* list) {
    if (!list || list->size == 0){ return this; }
    if (this->size == 0){
        *this = *list;
        return this;
    }
    this->tail->next = list->head;
    this->tail = list->tail;
    this->size += list->size;
    list->head = nullptr;
    list->tail = nullptr;
    list->size = 0;
    return this;
}

template<typename T>
LinkedList<T>::~LinkedList() {
    Node* cur = head;
    while (cur) {
        Node* tmp = cur;
        cur = cur->next;
        delete tmp;
    }
    head = nullptr;
    tail = nullptr;
    size = 0;
}

#endif 
