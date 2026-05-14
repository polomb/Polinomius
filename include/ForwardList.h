#ifndef FORWARD_LIST_H
#define FORWARD_LIST_H

#include <cstddef>
#include <stdexcept>
#include <utility>

template <typename T>
class ForwardListV;

namespace Task {
template <typename T>
void printReverse(const ForwardListV<T>&);
}

template <class T>
class ForwardListV {
private:
    struct Node {
        T data;
        Node* next = nullptr;

        explicit Node(const T& elem) : data(elem) {}
        explicit Node(T&& elem) : data(std::move(elem)) {}
    };

    Node* first = nullptr;
    std::size_t listSize = 0;

public:
    ForwardListV() = default;

    ForwardListV(std::size_t n, const T& elem = T{}) {
        if (n == 0) {
            return;
        }
        first = new Node(elem);
        Node* cur = first;
        for (std::size_t i = 1; i < n; ++i) {
            cur->next = new Node(elem);
            cur = cur->next;
        }
        listSize = n;
    }

    ForwardListV(const ForwardListV& other) : first(nullptr), listSize(0) {
        Node* current = other.first;
        Node* last = nullptr;
        while (current) {
            Node* newNode = new Node(current->data);
            if (!first) {
                first = newNode;
            } else {
                last->next = newNode;
            }
            last = newNode;
            current = current->next;
        }
        listSize = other.listSize;
    }

    ForwardListV& operator=(const ForwardListV& other) {
        if (this == &other) {
            return *this;
        }
        clear();
        Node* current = other.first;
        Node* last = nullptr;
        while (current) {
            Node* newNode = new Node(current->data);
            if (!first) {
                first = newNode;
            } else {
                last->next = newNode;
            }
            last = newNode;
            current = current->next;
        }
        listSize = other.listSize;
        return *this;
    }

    ForwardListV(ForwardListV&& other) noexcept : first(other.first), listSize(other.listSize) {
        other.first = nullptr;
        other.listSize = 0;
    }

    ForwardListV& operator=(ForwardListV&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        clear();
        first = other.first;
        listSize = other.listSize;
        other.first = nullptr;
        other.listSize = 0;
        return *this;
    }

    ~ForwardListV() { clear(); }

    T& operator[](std::size_t index) {
        if (index >= listSize) {
            throw std::out_of_range("ForwardListV::operator[] index out of range");
        }
        Node* cur = first;
        for (std::size_t i = 0; i < index; ++i) {
            cur = cur->next;
        }
        return cur->data;
    }

    const T& operator[](std::size_t index) const {
        if (index >= listSize) {
            throw std::out_of_range("ForwardListV::operator[] index out of range");
        }
        Node* cur = first;
        for (std::size_t i = 0; i < index; ++i) {
            cur = cur->next;
        }
        return cur->data;
    }

    T& at(std::size_t index) {
        if (index >= listSize) {
            throw std::out_of_range("ForwardListV::at index out of range");
        }
        Node* cur = first;
        for (std::size_t i = 0; i < index; ++i) {
            cur = cur->next;
        }
        return cur->data;
    }

    const T& at(std::size_t index) const {
        if (index >= listSize) {
            throw std::out_of_range("ForwardListV::at index out of range");
        }
        Node* cur = first;
        for (std::size_t i = 0; i < index; ++i) {
            cur = cur->next;
        }
        return cur->data;
    }

    bool empty() const noexcept { return first == nullptr; }
    std::size_t size() const noexcept { return listSize; }

    void clear() {
        Node* cur = first;
        while (cur) {
            Node* tmp = cur->next;
            delete cur;
            cur = tmp;
        }
        first = nullptr;
        listSize = 0;
    }

    T& front() {
        if (empty()) {
            throw std::out_of_range("ForwardListV::front");
        }
        return first->data;
    }

    const T& front() const {
        if (empty()) {
            throw std::out_of_range("ForwardListV::front");
        }
        return first->data;
    }

    class Iterator {
    public:
        Node* iterator = nullptr;

        Iterator() = default;
        explicit Iterator(Node* node) : iterator(node) {}

        T& operator*() const { return iterator->data; }
        T* operator->() const { return &iterator->data; }

        Iterator& operator++() {
            iterator = iterator->next;
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            iterator = iterator->next;
            return tmp;
        }

        bool operator==(const Iterator& value) const { return iterator == value.iterator; }
        bool operator!=(const Iterator& value) const { return !(*this == value); }
    };

    Iterator begin() { return Iterator(first); }
    Iterator end() { return Iterator(nullptr); }

    Iterator push_front(const T& elem) {
        Node* tmp = new Node(elem);
        tmp->next = first;
        first = tmp;
        ++listSize;
        return Iterator(first);
    }

    Iterator push_front(T&& elem) {
        Node* tmp = new Node(std::move(elem));
        tmp->next = first;
        first = tmp;
        ++listSize;
        return Iterator(first);
    }

    Iterator pop_front() {
        if (!first) {
            return end();
        }
        Node* tmp = first->next;
        delete first;
        first = tmp;
        --listSize;
        return Iterator(first);
    }

    Iterator insert_after(Iterator prev, const T& elem) {
        if (prev == end()) {
            return push_front(elem);
        }
        Node* tmp = new Node(elem);
        tmp->next = prev.iterator->next;
        prev.iterator->next = tmp;
        ++listSize;
        return Iterator(tmp);
    }

    Iterator erase_after(Iterator prev) {
        if (prev == end()) {
            return pop_front();
        }
        Node* tmp = prev.iterator->next;
        if (!tmp) {
            return end();
        }
        prev.iterator->next = tmp->next;
        delete tmp;
        --listSize;
        return Iterator(prev.iterator->next);
    }

    template <typename U>
    friend void Task::printReverse(const ForwardListV<U>&);
};

#endif  // FORWARD_LIST_H
