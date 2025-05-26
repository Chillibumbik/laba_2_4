#pragma once
#include <iostream>

template <typename T>
struct Node {
    int key;
    T value;
    Node* left;
    Node* right;

    Node(int k, T v) : key(k), value(v), left(nullptr), right(nullptr) {}
};

template <typename T>
class BinaryTree {
private:
    Node<T>* root = nullptr;

    void insert(Node<T>*& node, int key, const T& value);
    Node<T>* search(Node<T>* node, int key) const;
    Node<T>* getMin(Node<T>* node) const;
    Node<T>* getMax(Node<T>* node) const;
    void destroy(Node<T>* node);

public:
    ~BinaryTree();

    void insert(int key, const T& value);
    T* search(int key) const;
    T* getMin() const;
    T* getMax() const;
};

// -------------------- Реализация --------------------

template <typename T>
BinaryTree<T>::~BinaryTree() {
    destroy(root);
}

template <typename T>
void BinaryTree<T>::destroy(Node<T>* node) {
    if (!node) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

template <typename T>
void BinaryTree<T>::insert(int key, const T& value) {
    insert(root, key, value);
}

template <typename T>
void BinaryTree<T>::insert(Node<T>*& node, int key, const T& value) {
    if (!node) {
        node = new Node<T>(key, value);
        return;
    }

    if (key < node->key)
        insert(node->left, key, value);
    else if (key > node->key)
        insert(node->right, key, value);
    else
        node->value = value; // если ключ уже есть — обновляем значение
}

template <typename T>
T* BinaryTree<T>::search(int key) const {
    Node<T>* res = search(root, key);
    return res ? &res->value : nullptr;
}

template <typename T>
Node<T>* BinaryTree<T>::search(Node<T>* node, int key) const {
    if (!node || node->key == key) return node;
    return (key < node->key) ? search(node->left, key) : search(node->right);
}

template <typename T>
T* BinaryTree<T>::getMin() const {
    Node<T>* minNode = getMin(root);
    return minNode ? &minNode->value : nullptr;
}

template <typename T>
Node<T>* BinaryTree<T>::getMin(Node<T>* node) const {
    if (!node || !node->left) return node;
    return getMin(node->left);
}

template <typename T>
T* BinaryTree<T>::getMax() const {
    Node<T>* maxNode = getMax(root);
    return maxNode ? &maxNode->value : nullptr;
}

template <typename T>
Node<T>* BinaryTree<T>::getMax(Node<T>* node) const {
    if (!node || !node->right) return node;
    return getMax(node->right);
}
