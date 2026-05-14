#ifndef __List__
#define __List__

#include <iostream>
#include <vector>

template<class T>
class ForwardList
{
    struct Node
    {
        T data;
        Node* next = nullptr;
        //
        //Node() : data(), next() {}
        Node(const T& elem) : data(elem), next(nullptr) {}
        Node(T&& elem) : data(std::move(elem)), next(nullptr) {}
    };
    Node* first = nullptr;
    size_t listSize = 0;
public:
    ForwardList() = default;
    ForwardList(size_t n, const T& elem = T{})
    {
        if (n == 0) return;
        first = new Node(elem);
        Node* cur = first;
        for (size_t i = 1; i < n; i++)
        {
            cur->next = new Node(elem);
            cur = cur->next;
        }
        listSize = n;
    }
    //Правило пяти?
    ForwardList(const ForwardList& other)
    {
        if (!other.empty())
        {
            Node* cur = other.first;
            Node* last = nullptr;
            while (cur != nullptr)
            {
                Node* newNode = new Node(cur->data);
                if (last == nullptr)
                {
                    first = newNode;
                }
                else
                {
                    last->next = newNode;
                }
                last = newNode;
                cur = cur->next;
            }
            listSize = other.listSize;
        }
    }
    ForwardList& operator=(const ForwardList& other)
    {
        if (this != &other)
        {
            clear();
            Node* cur = other.first;
            Node* last = nullptr;
            while (cur != nullptr)
            {
                Node* newNode = new Node(cur->data);
                if (last == nullptr)
                {
                    first = newNode;
                }
                else
                {
                    last->next = newNode;
                }
                last = newNode;
                cur = cur->next;
            }
            listSize = other.listSize;
        }
        return *this;
    }
    ~ForwardList()
    {
        clear();
    }
    ForwardList(ForwardList&& other) noexcept : first(other.first), listSize(other.listSize)
    {
        other.first = nullptr;
        other.listSize = 0;
    }
    ForwardList& operator=(ForwardList&& other) noexcept
    {
        if (this != &other)
        {
            clear();
            first = other.first;
            listSize = other.listSize;
            other.first = nullptr;
            other.listSize = 0;
        }
        return *this;
    }
    //
    T& operator[](size_t index)
    {
        Node* cur = first;
        for (size_t i = 0; i < index; cur = cur->next, i++);
        return cur->data;
    }
    const T& operator[](size_t index) const
    {
        Node* cur = first;
        for (size_t i = 0; i < index; cur = cur->next, i++);
        return cur->data;
    }
    T& at(size_t index)
    {
        Node* cur = first;
        for (size_t i = 0; cur && i < index; cur = cur->next, i++);
        if (!cur) { throw std::out_of_range("Error_T& at(size_t index): nullptr"); }
        return cur->data;
    }
    const T& at(size_t index) const
    {
        Node* cur = first;
        for (size_t i = 0; cur && i < index; cur = cur->next, i++);
        if (!cur) { throw std::out_of_range("Error_const T& at(size_t index) const: nullptr"); }
        return cur->data;
    }
    //
    bool empty() const noexcept
    {
        return (first == nullptr);
    }
    void clear()
    {
        Node* cur = first;
        while (cur)
        {
            Node* tmp = cur->next;
            delete cur;
            cur = tmp;
        }
        first = nullptr;
        listSize = 0;
    }
    size_t size() const noexcept
    {
        return listSize;
    }
    //
    T& front()
    {
        if (empty())
        {
            throw std::out_of_range("Error_T& front():List is empty");
        }
        return first->data;
    }
    const T& front() const
    {
        if (empty())
        {
            throw std::out_of_range("Error_const T& front() const:List is empty");
        }
        return first->data;
    }
    //
    class Iterator
    {
    public:
        Node* iter;
        //
        Iterator(Node* node = nullptr) : iter(node) {}
        //
        T& operator*() const
        {
            return iter->data;
        }
        T* operator->() const
        {
            return &iter->data;
        }
        //
        Iterator& operator++()
        {
            iter = iter->next;
            return *this;
        }
        Iterator operator++(int)
        {
            Iterator temp = *this;
            iter = iter->next;
            return temp;
        }
        //
        bool operator==(const Iterator& other) const
        {
            return iter == other.iter;
        }
        bool operator!=(const Iterator& other) const
        {
            return iter != other.iter;
        }
        //
    };
    //
    Iterator push_front(const T& elem)
    {
        Node* tmp = new Node(elem);
        tmp->next = first;
        first = tmp;
        listSize++;
        return first;
    }
    Iterator push_front(T&& elem)
    {
        Node* tmp = new Node(std::forward(elem));
        tmp->next = first;
        first = tmp;
        listSize++;
        return first;
    }
    Iterator pop_front()
    {
        if (!first || listSize == 0) return Iterator(nullptr);
        Node* tmp = first->next;
        delete first;
        first = tmp;
        listSize--;
        return first;
    }
    //
    Iterator begin() { return Iterator(first); }
    Iterator end() { return Iterator(nullptr); }
    Iterator atIndex(size_t index)
    {
        Node* cur = first;
        for (size_t i = 0; cur && i < index; cur = cur->next, i++);
        if (!cur) { throw std::out_of_range("Error_T& at(size_t index): nullptr"); }
        return Iterator(cur);
    }
    template<class Iterator>
    Iterator insert_after(Iterator prev, const T& elem)
    {
        if (prev == Iterator(nullptr)) return push_front(elem);
        Node* tmp = new Node(elem);
        tmp->next = prev.iter->next;
        prev.iter->next = tmp;
        listSize++;
        return Iterator(tmp);
    }
    template<class Iterator>
    Iterator erase_after(Iterator prev)
    {
        if (prev == Iterator(nullptr)) return pop_front();
        if (prev.iter->next)
        {
            Node* tmp = prev.iter->next->next;
            delete prev.iter->next;
            prev.iter->next = tmp;
            listSize--;
            return Iterator(prev.iter->next);
        }
        return Iterator(nullptr);
    }
    class ConstIterator
    {
    public:
        const Node* iter;

        ConstIterator(const Node* node = nullptr) : iter(node) {}

        const T& operator*() const
        {
            return iter->data;
        }

        const T* operator->() const
        {
            return &iter->data;
        }

        ConstIterator& operator++()
        {
            iter = iter->next;
            return *this;
        }

        ConstIterator operator++(int)
        {
            ConstIterator temp = *this;
            iter = iter->next;
            return temp;
        }

        bool operator==(const ConstIterator& other) const
        {
            return iter == other.iter;
        }

        bool operator!=(const ConstIterator& other) const
        {
            return iter != other.iter;
        }
    };
    ConstIterator begin() const { return ConstIterator(first); }
    ConstIterator end() const { return ConstIterator(nullptr); }
};

#endif

