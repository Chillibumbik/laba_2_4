#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <functional>
#include <stdexcept>
#include "Errors.hpp"
#include <iomanip>

template<typename T>
class BinaryTree {
private:
    struct Node {
        int key;
        T value;
        Node* left;
        Node* right;

        Node(int k, const T& v) : key(k), value(v), left(nullptr), right(nullptr) {} 
    };

    Node* root;
    int size;

    void destroy(Node* node);
    Node* copy(Node* node) const;
    Node* insert(Node* node, int key, const T& value);
    Node* remove(Node* node, int key, bool& success);
    Node* search(Node* node, int key) const;
    Node* getMinNode(Node* node) const;
    Node* getMaxNode(Node* node) const;

    void traverse(Node* node, const std::string& order, std::function<void(const T&)> func) const;
    void traverse(std::function<void(int, const T&)> func) const;
    void traverse(Node* node, std::function<void(int, const T&)> func) const;

    std::string serializeNode(Node* node) const;
    static Node* parseNode(const std::string& s, size_t& pos);


    bool equals(Node* a, Node* b) const;
    bool containsSubtree(Node* root, Node* sub) const;
    Node* find(Node* node, const T& value) const;

    Node* buildBalancedTree(const std::vector<std::pair<int, T>>& nodes, int start, int end);
    void inOrderCollect(Node* node, std::vector<std::pair<int, T>>& out) const;

    int getDepth(Node* node) const;

    void printNode(Node* node, int indent) const;

    bool isValidBST(Node* node, const int* minKey, const int* maxKey) const;


    // добавить приватный метод балансировки дерева

public:
    BinaryTree();
    BinaryTree(const BinaryTree<T>& other);
    ~BinaryTree();

    void insert(int key, const T& value);
    bool remove(int key);
    T* search(int key) const;
    T getMin() const;
    T getMax() const;

    void traverseKLP(std::function<void(const T&)> func) const;
    void traverseKPL(std::function<void(const T&)> func) const;
    void traverseLPK(std::function<void(const T&)> func) const;
    void traverseLKP(std::function<void(const T&)> func) const;
    void traversePLK(std::function<void(const T&)> func) const;
    void traversePKL(std::function<void(const T&)> func) const;

    BinaryTree<T> map(std::function<T(const T&)> f) const;
    BinaryTree<T> where(std::function<bool(const T&)> p) const;
    BinaryTree<T> merge(const BinaryTree<T>& other) const;
    BinaryTree<T> extractSubtree(int key) const;

    bool containsSubtree(const BinaryTree<T>& sub) const;
    bool containsNode(const T& value) const;

    std::string toString() const;
    static BinaryTree<T> fromString(const std::string& str);
    bool isValidTreeString(const std::string& s);

    T* findByPath(const std::string& path) const;
    T* findByRelativePath(const std::string& path, const T& from) const;

    void balance();
    int GetDepth() const;

    BinaryTree<T>& operator=(const BinaryTree<T>& other);

    void PrintTree() const;

    bool operator==(const BinaryTree<T>& other) const;
    bool operator!=(const BinaryTree<T>& other) const;

};



template<typename T>
BinaryTree<T>::BinaryTree() : root(nullptr), size(0) {}

template<typename T>
BinaryTree<T>::BinaryTree(const BinaryTree<T>& other) : root(copy(other.root)), size(other.size) {}

template<typename T>
BinaryTree<T>::~BinaryTree() {
    destroy(root);
}

template<typename T>
void BinaryTree<T>::destroy(Node* node) {
    if (!node) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

template<typename T>
typename BinaryTree<T>::Node* BinaryTree<T>::insert(Node* node, int key, const T& value) {
    if (!node) {
        ++size;
        return new Node(key, value);
    }
    if (key < node->key) {
        node->left = insert(node->left, key, value);
    } else if (key > node->key) {
        node->right = insert(node->right, key, value);
    } else {
        node->value = value;
    }
    return node;
}


template<typename T>
void BinaryTree<T>::insert(int key, const T& value) {
    root = insert(root, key, value);
}

template<typename T>
typename BinaryTree<T>::Node* BinaryTree<T>::search(Node* node, int key) const {
    if (!node) return nullptr;
    if (key == node->key) return node;
    if (key < node->key) return search(node->left, key);
    return search(node->right, key);
}

template<typename T>
T* BinaryTree<T>::search(int key) const {
    Node* res = search(root, key);
    return res ? &res->value : nullptr;
}

template<typename T>
typename BinaryTree<T>::Node* BinaryTree<T>::getMinNode(Node* node) const {
    if (!node) return nullptr;
    while (node->left) node = node->left;
    return node;
}

template<typename T>
typename BinaryTree<T>::Node* BinaryTree<T>::getMaxNode(Node* node) const {
    if (!node) return nullptr;
    while (node->right) node = node->right;
    return node;
}

template<typename T>
T BinaryTree<T>::getMin() const {
    Node* min = getMinNode(root);
    if (!min) throw Errors::TreeEmpty();
    return min->value;
}

template<typename T>
T BinaryTree<T>::getMax() const {
    Node* max = getMaxNode(root);
    if (!max) throw Errors::TreeEmpty();
    return max->value;
}

template<typename T>
typename BinaryTree<T>::Node* BinaryTree<T>::remove(Node* node, int key, bool& success) {
    if (!node) return nullptr;
    if (key < node->key)
        node->left = remove(node->left, key, success);
    else if (key > node->key)
        node->right = remove(node->right, key, success);
    else {
        success = true;
        --size;
        if (!node->left) {
            Node* temp = node->right;
            delete node;
            return temp;
        }
        if (!node->right) {
            Node* temp = node->left;
            delete node;
            return temp;
        }
        Node* minRight = getMinNode(node->right);
        node->key = minRight->key;
        node->value = minRight->value;
        node->right = remove(node->right, minRight->key, success);
    }
    return node;
}

template<typename T>
bool BinaryTree<T>::remove(int key) {
    bool success = false;
    root = remove(root, key, success);
    return success;
}

template<typename T>
void BinaryTree<T>::traverse(Node* node, const std::string& order, std::function<void(const T&)> func) const {
    if (!node) return;
    if (order == "KLP") { func(node->value); traverse(node->left, order, func); traverse(node->right, order, func); }
    else if (order == "KPL") { func(node->value); traverse(node->right, order, func); traverse(node->left, order, func); }
    else if (order == "LPK") { traverse(node->left, order, func); traverse(node->right, order, func); func(node->value); }
    else if (order == "LKP") { traverse(node->left, order, func); func(node->value); traverse(node->right, order, func); }
    else if (order == "PLK") { traverse(node->right, order, func); traverse(node->left, order, func); func(node->value); }
    else if (order == "PKL") { traverse(node->right, order, func); func(node->value); traverse(node->left, order, func); }
    else throw Errors::UnknownOrder(order);
}

template<typename T>
void BinaryTree<T>::traverse(std::function<void(int, const T&)> func) const {
    traverse(root, func);
}

template<typename T>
void BinaryTree<T>::traverse(Node* node, std::function<void(int, const T&)> func) const {
    if (!node) return;
    func(node->key, node->value);
    traverse(node->left, func);
    traverse(node->right, func);
}


template<typename T> void BinaryTree<T>::traverseKLP(std::function<void(const T&)> func) const { traverse(root, "KLP", func); }
template<typename T> void BinaryTree<T>::traverseKPL(std::function<void(const T&)> func) const { traverse(root, "KPL", func); }
template<typename T> void BinaryTree<T>::traverseLPK(std::function<void(const T&)> func) const { traverse(root, "LPK", func); }
template<typename T> void BinaryTree<T>::traverseLKP(std::function<void(const T&)> func) const { traverse(root, "LKP", func); }
template<typename T> void BinaryTree<T>::traversePLK(std::function<void(const T&)> func) const { traverse(root, "PLK", func); }
template<typename T> void BinaryTree<T>::traversePKL(std::function<void(const T&)> func) const { traverse(root, "PKL", func); }

template<typename T>
BinaryTree<T> BinaryTree<T>::map(std::function<T(const T&)> f) const {
    BinaryTree<T> result;
    traverseKLP([&](const T& val) { result.insert(val, f(val)); });
    return result;
}

template<typename T>
BinaryTree<T> BinaryTree<T>::where(std::function<bool(const T&)> p) const {
    BinaryTree<T> result;
    traverseKLP([&](const T& val) {
        if (p(val)) result.insert(val, val);
    });
    return result;
}

template<typename T>
BinaryTree<T> BinaryTree<T>::merge(const BinaryTree<T>& other) const {
    BinaryTree<T> result;
    traverse([&result](int key, const T& val) { // обходим по первому дереву(нет прямого доступа к ключу, поэтому используем traverse)
        result.insert(key, val);
    });
    other.traverse([&result](int key, const T& val) { // обходим по второму дереву
        result.insert(key, val);
    });
    result.balance(); // балансируем после слияния
    return result;
}


template<typename T>
typename BinaryTree<T>::Node* BinaryTree<T>::copy(Node* node) const {
    if (!node) return nullptr;
    Node* newNode = new Node(node->key, node->value);
    newNode->left = copy(node->left);
    newNode->right = copy(node->right);
    return newNode;
}

template<typename T>
BinaryTree<T> BinaryTree<T>::extractSubtree(int key) const {
    Node* found = search(root, key);
    if (!found) throw Errors::KeyNotFound();
    BinaryTree<T> result;
    result.root = copy(found);
    return result;
}

template<typename T>
bool BinaryTree<T>::equals(Node* a, Node* b) const {
    if (!a && !b) return true;
    if (!a || !b) return false;
    return a->value == b->value &&
           equals(a->left, b->left) &&
           equals(a->right, b->right);
}

template<typename T>
bool BinaryTree<T>::containsSubtree(Node* root, Node* sub) const {
    if (!root) return false;
    if (equals(root, sub)) return true;
    return containsSubtree(root->left, sub) || containsSubtree(root->right, sub);
}

template<typename T>
bool BinaryTree<T>::containsSubtree(const BinaryTree<T>& sub) const {
    return containsSubtree(root, sub.root);
}

template<typename T>
bool BinaryTree<T>::containsNode(const T& value) const {
    return find(root, value) != nullptr;
}

template<typename T>
typename BinaryTree<T>::Node* BinaryTree<T>::find(Node* node, const T& value) const {
    if (!node) return nullptr;
    if (node->value == value) return node;
    Node* l = find(node->left, value);
    if (l) return l;
    return find(node->right, value);
}

template<typename T>
std::string BinaryTree<T>::toString() const {
    return serializeNode(root);
}

template<typename T>
std::string BinaryTree<T>::serializeNode(Node* node) const {
    if (!node) return "()"; // Пустое поддерево

    std::ostringstream out;
    out << "("; // откр
    out << serializeNode(node->left); // лево

    // Печатаем key и value
    out << node->key << ":";
    if constexpr (std::is_same_v<T, std::function<double(double)>>) {
        out << "<function>";
    } else {
        out << node->value;
    }

    out << serializeNode(node->right); // право
    out << ")"; // закр

    return out.str();
}

template<typename T>
bool BinaryTree<T>::isValidTreeString(const std::string& s) {
    size_t pos = 0;
    try {
        Node* node = parseNode(s, pos);
        if (pos != s.size()) throw Errors::ParseError(); // не всё прочитано

        bool valid = isValidBST(node, nullptr, nullptr);
        destroy(node); // очищаем временное дерево

        return valid;
    } catch (...) {
        return false;
    }
}

template<typename T>
bool BinaryTree<T>::isValidBST(Node* node, const int* minKey, const int* maxKey) const {
    if (!node) return true;

    if ((minKey && node->key <= *minKey) || (maxKey && node->key >= *maxKey))
        return false;

    return isValidBST(node->left, minKey, &node->key) &&
           isValidBST(node->right, &node->key, maxKey);
}



template<typename T>
BinaryTree<T> BinaryTree<T>::fromString(const std::string& str) {
    size_t pos = 0;
    BinaryTree<T> tree;

    if (!tree.isValidTreeString(str)) {
        throw Errors::ParseError("Invalid tree string: structure or BST property violated.");
    }

    tree.root = tree.parseNode(str, pos);
    return tree;
}


template<typename T>
typename BinaryTree<T>::Node* BinaryTree<T>::parseNode(const std::string& s, size_t& pos) {
/* 
(())5:5(())6:6()))
(())5:5()))
*/
    if (pos >= s.size() || s[pos] != '(') throw Errors::ParseError();
    ++pos;                           // пропустили '('

    // пустое поддерево 
    if (s[pos] == ')') { ++pos; return nullptr; }

    //  левое 
    Node* left = parseNode(s, pos);

    // key 
    std::string key_str;
    while (pos < s.size() && std::isdigit(s[pos]))
        key_str += s[pos++];
    if (key_str.empty() || s[pos++] != ':')
        throw Errors::ParseError();
    int key = std::stoi(key_str);

    //  value 
    std::string val_str;
    while (pos < s.size() && s[pos] != '(' && s[pos] != ')')
        val_str += s[pos++];
    std::istringstream vs(val_str);
    T value;
    vs >> value;

    // правое 
    Node* right = parseNode(s, pos);

    // закрывающая
    if (pos >= s.size() || s[pos] != ')') throw Errors::ParseError();
    ++pos;

    // собираем 
    Node* node = new Node(key, value);
    node->left  = left;
    node->right = right;
    return node;
}



template<typename T>
T* BinaryTree<T>::findByPath(const std::string& path) const {
    Node* node = root;
    for (char c : path) {
        if (!node) return nullptr;
        if (c == 'L') node = node->left;
        else if (c == 'P') node = node->right;
        else throw Errors::InvalidPath();
    }
    return node ? &node->value : nullptr;
}

template<typename T>
T* BinaryTree<T>::findByRelativePath(const std::string& path, const T& from) const {
    Node* node = find(root, from);
    if (!node) return nullptr;
    for (char c : path) {
        if (!node) return nullptr;
        if (c == 'L') node = node->left;
        else if (c == 'P') node = node->right;
        else throw Errors::InvalidPath();
    }
    return node ? &node->value : nullptr;
}

template<typename T>
typename BinaryTree<T>::Node* BinaryTree<T>::buildBalancedTree(const std::vector<std::pair<int, T>>& nodes, int start, int end) {
    if (start > end) return nullptr;
    int mid = (start + end) / 2; // среднее м-у стартом и концом
    Node* node = new Node(nodes[mid].first, nodes[mid].second); // берется из словаря nodes средний нод и его ключ(first) и значение (second), которые потом в новый нод идут
    node->left = buildBalancedTree(nodes, start, mid - 1); // рекурсивно создаем узел для середины слева
    node->right = buildBalancedTree(nodes, mid + 1, end); // рекурсивно создаем для середины справа
    return node; // возвращаем узел тогда, когда у нас  не осталось возможных потомков (старт > енд)
}

template<typename T>
void BinaryTree<T>::inOrderCollect(Node* node, std::vector<std::pair<int, T>>& out) const {
    if (!node) return; // если нет нода
    inOrderCollect(node->left, out); 
    out.push_back({node->key, node->value}); // закидываем в словарь пару {ключ, значение}
    inOrderCollect(node->right, out);
    //рекурсивно делаем для левой и правой части(ин ордер)
}

template<typename T>
void BinaryTree<T>::balance() {
    std::vector<std::pair<int, T>> nodes; // словарь узлов
    inOrderCollect(root, nodes); // закидываем все узлы по KLP в словарь
    destroy(root); // уничтожаем дерево(несбалансированное)
    root = buildBalancedTree(nodes, 0, nodes.size() - 1); // новый корень для дерева(сбалансированное)
}

template<typename T>
int BinaryTree<T>::getDepth(Node* node) const {
    if (!node) return 0;
    return 1 + std::max(getDepth(node->left), getDepth(node->right));
}

template<typename T>
int BinaryTree<T>::GetDepth() const {
    return getDepth(root);
}
    
template<typename T>
BinaryTree<T>& BinaryTree<T>::operator=(const BinaryTree<T>& other) {
    if (this != &other) {
        destroy(root);
        root = copy(other.root);
        size = other.size;
    }
    return *this;
}

template<typename T>
void BinaryTree<T>::PrintTree() const {
    printNode(root, 0);
}

template<typename T>
void BinaryTree<T>::printNode(Node* node, int indent) const {
    if (node) {
        if (node->right) printNode(node->right, indent + 5);
        
        if (indent) std::cout << std::setw(indent) << ' ';
        
        std::cout << node->key << ": ";
        
        if constexpr (std::is_same_v<T, std::function<double(double)>>) {
            std::cout << "<function>" << std::endl;
        } else {
            std::cout << node->value << std::endl;
        }

        if (node->left) printNode(node->left, indent + 5);
    }
}


template<typename T>
bool BinaryTree<T>::operator==(const BinaryTree<T>& other) const {
    return equals(this->root, other.root);
}

template<typename T>
bool BinaryTree<T>::operator!=(const BinaryTree<T>& other) const {
    return !(*this == other);
}