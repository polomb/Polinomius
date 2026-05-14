#pragma once
#include <stdexcept>
#include <algorithm>
#include <functional>
#include <iterator>
#include <string>
#include <sstream>
#include "ForwardList.h"

/**
 * AVL-дерево — это самобалансирующееся BST, в котором для каждого
 *  узла v выполняется:
 *      |h(left(v)) - h(right(v))| <= 1
 *
 *  balance factor b(v) = h(right) - h(left)
 *  -1,0,1
 *
 *  TValue = void* по умолчанию → старый код AVLTree<int> компилируется без изменений.
 *  Новый код может использовать AVLTree<int, std::string>.
 */

template <typename Key, typename TValue = void*, typename Compare = std::less<Key>>
class AVLTree {
public:
    struct Node {
        Key    key;
        TValue value;
        Node* left = nullptr;
        Node* right = nullptr;
        int    height = 1;

        Node(const Key& k, const TValue& v) : key(k), value(v) {}
        explicit Node(const Key& k) : key(k), value{} {}
    };

    AVLTree() = default;
    ~AVLTree() { clear(); }

    AVLTree(const AVLTree&) = delete;
    AVLTree& operator=(const AVLTree&) = delete;

    // insert(key, value) — новый вариант с значением
    void insert(const Key& key, const TValue& value) {
        root_ = insertNode(root_, key, value);
        size_++;
    }

    // insert(key) — старый вариант без значения (обратная совместимость)
    void insert(const Key& key) {
        root_ = insertNode(root_, key, TValue{});
        size_++;
    }

    void erase(const Key& key) {
        bool removed = false;
        root_ = eraseNode(root_, key, removed);
        if (removed) size_--;
    }
    int  size()   const { return size_; }
    bool empty()  const { return size_ == 0; }
    int  height() const { return getHeight(root_); }
    void clear() { destroyTree(root_); root_ = nullptr; size_ = 0; }

    // operator[](int) — доступ по in-order индексу, возвращает ключ
    const Key& operator[](int index) const {
        if (index < 0 || index >= size_) {
            throw std::out_of_range("Index out of range");
        }

        Node* current = root_;

        while (current) {
            int leftCount = getNodeCount(current->left);

            if (index < leftCount) {
                current = current->left;
            }
            else if (index > leftCount) {
                index -= leftCount + 1;
                current = current->right;
            }
            else {
                return current->key;
            }
        }

        throw std::out_of_range("Index not found");
    }

    TValue& operator[](const Key& key) {
        Node* node = findNode(root_, key);
        if (node) {
            return node->value;
        }
        insert(key, TValue{});
        node = findNode(root_, key);
        return node->value;
    }

    class Iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = const Key;
        using reference = const Key&;
        using pointer = const Key*;

        Iterator() = default;
        explicit Iterator(Node* root) { pushLeft(root); }

        reference operator*()  const { return stack_.front()->key; }
        pointer   operator->() const { return &stack_.front()->key; }

        // Доступ к значению текущего узла
        TValue& value() const { return stack_.front()->value; }

        Iterator& operator++() {
            Node* node = stack_.front();
            stack_.pop_front();
            pushLeft(node->right);
            return *this;
        }

        bool operator==(const Iterator& other) const {
            if (stack_.empty() && other.stack_.empty()) return true;
            if (stack_.empty() || other.stack_.empty()) return false;
            return stack_.front() == other.stack_.front();
        }
        bool operator!=(const Iterator& other) const { return !(*this == other); }

    private:
        ForwardListV<Node*> stack_;

        void pushLeft(Node* node) {
            while (node) {
                stack_.push_front(node);
                node = node->left;
            }
        }

        friend class AVLTree<Key, TValue, Compare>;
    };

    Iterator begin() const { return Iterator(root_); }
    Iterator end()   const { return Iterator(); }

    Iterator find(const Key& key) const {
        if (findNode(root_, key) != nullptr)
            return this->begin();
        else
            return this->end();
    }

    // find_iter — поиск по ключу, возвращает итератор на элемент или end()
    Iterator find_iter(const Key& key) const {
        if (!findNode(root_, key)) return end();
        Iterator it;
        fillIteratorStack(root_, key, it.stack_);
        return it;
    }

    bool isValid() const { return checkAVL(root_); }

    const Node* getRoot() const { return root_; }

private:
    Node* root_ = nullptr;
    int     size_ = 0;
    Compare cmp_;

    int getHeight(const Node* n) const { return n ? n->height : 0; }

    // balance factor: b = h(right) - h(left)
    // b > 0   правое поддерево выше
    // b < 0   левое поддерево выше
    int getBalance(const Node* n) const {
        return n ? getHeight(n->right) - getHeight(n->left) : 0;
    }

    void updateHeight(Node* n) {
        if (n)
            n->height = 1 + std::max(getHeight(n->left), getHeight(n->right));
    }

    //  LL (Left-Left)   : balance > +1, левый ребёнок balance >= 0
    //                     → один правый поворот
    //
    //  RR (Right-Right) : balance < -1, правый ребёнок balance <= 0
    //                     → один левый поворот
    //
    //  LR (Left-Right)  : balance > +1, левый ребёнок balance < 0
    //                     → сначала левый поворот вокруг левого ребёнка,
    //                       потом правый поворот вокруг текущего
    //
    //  RL (Right-Left)  : balance < -1, правый ребёнок balance > 0
    //                     → сначала правый поворот вокруг правого ребёнка,
    //                       потом левый поворот вокруг текущего

    Node* rotateRight(Node* y) {
        Node* x = y->left;   // шаг 1: x — новый корень
        Node* z = x->right;  // шаг 2: z — правый ребёнок x (будет перемещён)

        x->right = y;        // шаг 3: y "опускается" вправо под x
        y->left = z;        // шаг 4: z "переезжает" стать левым ребёнком y

        updateHeight(y);     // шаг 5a: y теперь ниже в дереве — обновляем первым
        updateHeight(x);     // шаг 5b: x — новый корень — обновляем после y

        return x;            // шаг 6: возвращаем новый корень поддерева
    }

    Node* rotateLeft(Node* y) {
        Node* x = y->right;  // шаг 1: x — новый корень
        Node* z = x->left;   // шаг 2: z — левый ребёнок x (переедет к y)

        x->left = y;        // шаг 3: y "опускается" влево под x
        y->right = z;        // шаг 4: z "переезжает" стать правым ребёнком y

        updateHeight(y);     // шаг 5a
        updateHeight(x);     // шаг 5b

        return x;            // шаг 6
    }

    Node* rebalance(Node* z) {
        updateHeight(z);
        int bal = getBalance(z);
        if (bal < -1 && getBalance(z->left) <= 0) {
            return rotateRight(z);
        }
        if (bal < -1 && getBalance(z->left) > 0) {
            z->left = rotateLeft(z->left);
            return rotateRight(z);
        }
        if (bal > 1 && getBalance(z->right) >= 0) {
            return rotateLeft(z);
        }
        if (bal > 1 && getBalance(z->right) < 0) {
            z->right = rotateRight(z->right);
            return rotateLeft(z);
        }
        return z;
    }

    Node* insertNode(Node* node, const Key& key, const TValue& value) {
        if (!node) return new Node(key, value);

        if (cmp_(key, node->key)) {
            node->left = insertNode(node->left, key, value);
        }
        else if (cmp_(node->key, key)) {
            node->right = insertNode(node->right, key, value);
        }
        else {
            throw std::runtime_error("Duplicate key!");
        }

        return rebalance(node);
    }

    Node* eraseNode(Node* node, const Key& key, bool& removed) {
        if (!node) return nullptr;
        if (cmp_(key, node->key)) {
            node->left = eraseNode(node->left, key, removed);
        }
        else if (cmp_(node->key, key)) {
            node->right = eraseNode(node->right, key, removed);
        }
        else {
            removed = true;

            if (!node->left || !node->right) {
                Node* child = node->left ? node->left : node->right;
                delete node;
                return child;
            }
            else {
                Node* successor = mostLeftChild(node->right);
                node->key = successor->key;
                node->value = successor->value;

                bool dummy = false;
                node->right = eraseNode(node->right, successor->key, dummy);
            }
        }

        return rebalance(node);
    }

    // поиска самого левого узла
    Node* mostLeftChild(Node* node) const {
        while (node->left) node = node->left;
        return node;
    }

    // для поиска узла по ключу
    Node* findNode(Node* node, const Key& key) const {
        if (!node) return nullptr;
        if (cmp_(key, node->key)) return findNode(node->left, key);
        if (cmp_(node->key, key)) return findNode(node->right, key);
        return node;
    }

    // для удаления дерева(рекурсивно)
    void destroyTree(Node* node) {
        if (!node) return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

    // для отладки и тестирования
    bool checkAVL(const Node* node) const {
        if (!node) return true;
        int bal = getBalance(node);
        if (bal < -1 || bal > 1) return false;
        int expected = 1 + std::max(getHeight(node->left), getHeight(node->right));
        if (node->height != expected) return false;
        return checkAVL(node->left) && checkAVL(node->right);
    }

    int getNodeCount(const Node* node) const {
        if (!node) return 0;
        return 1 + getNodeCount(node->left) + getNodeCount(node->right);
    }

    void fillIteratorStack(Node* node, const Key& key,
        ForwardListV<Node*>& stack) const {
        if (!node) return;
        if (cmp_(key, node->key)) {
            stack.push_front(node);
            fillIteratorStack(node->left, key, stack);
        }
        else if (cmp_(node->key, key)) {
            fillIteratorStack(node->right, key, stack);
        }
        else {
            stack.push_front(node);
        }
    }
};