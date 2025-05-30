
#include <iostream>
#include <string>
#include <vector>
#include <complex>
#include <functional>
#include "BinaryTree.hpp"
#include "Users.hpp"
#include "Errors.hpp"

void ClearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int GetInt(const std::string& prompt = "") {
    int v;
    if (!prompt.empty()) std::cout << prompt;
    if (!(std::cin >> v)) {
        ClearInput();
        throw Errors::InvalidArgument();
    }
    return v;
}

template<typename T>
T GetTyped(const std::string& prompt = "Enter value: ") {
    std::cout << prompt;
    T val;
    if (!(std::cin >> val)) {
        ClearInput();
        throw Errors::InvalidArgument();
    }
    return val;
}

template<>
std::complex<double> GetTyped<std::complex<double>>(const std::string& prompt) {
    std::cout << prompt << " (real and imaginary part): ";
    double re, im;
    std::cin >> re >> im;
    return {re, im};
}

template<>
Student GetTyped<Student>(const std::string& prompt) {
    Student s;
    std::cin >> s;
    return s;
}

template<>
Teacher GetTyped<Teacher>(const std::string& prompt) {
    Teacher t;
    std::cin >> t;
    return t;
}

template<>
std::function<double(double)> GetTyped<std::function<double(double)>>(const std::string& prompt) {
    std::cout << prompt << " (select function)\n1. sin(x)\n2. x^2\n3. exp(x)\nChoice: ";
    int choice = GetInt();
    if (choice == 1) return [](double x) { return sin(x); };
    if (choice == 2) return [](double x) { return x * x; };
    if (choice == 3) return [](double x) { return exp(x); };
    throw Errors::InvalidArgument("Invalid function selection");
}

struct ITreeWrapper {
    virtual ~ITreeWrapper() = default;
    virtual std::string TypeName() const = 0;
    virtual void Menu(std::vector<ITreeWrapper*>&, std::vector<std::string>&) = 0;
};

template<typename T>
class TreeWrapper : public ITreeWrapper {
public:
    BinaryTree<T> tree;
    std::string typeName;

    TreeWrapper(std::string typeName_) : typeName(std::move(typeName_)) {}

    std::string TypeName() const override { return typeName; }

    void Menu(std::vector<ITreeWrapper*>& globalTrees, std::vector<std::string>& typeRegistry) override {
        while (true) {
            std::cout << "\n--- Tree Menu (" << typeName << ") ---\n"
                      << "1. Insert\n2. Search\n3. Min\n4. Max\n5. Remove\n"
                      << "6. Traverse (KLP)\n7. Merge with another\n8. Extract Subtree\n"
                      << "9. Balance Tree\n10. Back\n"
                      << "Choose: ";
            try {
                int ch = GetInt();
                switch (ch) {
                    case 1: { // insert
                        int key = GetInt("Key: ");
                        T val = GetTyped<T>("Value: ");
                        tree.insert(key, val);
                        break;
                    }
                    case 2: { // search
                        int key = GetInt("Key: ");
                        T* found = tree.search(key);
                        if (found) {
                            if constexpr (std::is_same_v<T, std::function<double(double)>>)
                                std::cout << "Found: f(1.0) = " << (*found)(1.0) << "\n";
                            else std::cout << "Found: " << *found << "\n";
                        } else std::cout << "Not found\n";
                        break;
                    }
                    case 3: { // min
                        if constexpr (std::is_same_v<T, std::function<double(double)>>)
                            std::cout << "Min: f(1.0) = " << tree.getMin()(1.0) << "\n";
                        else std::cout << "Min: " << tree.getMin() << "\n";
                        break;
                    }
                    case 4: { // max
                        if constexpr (std::is_same_v<T, std::function<double(double)>>)
                            std::cout << "Max: f(1.0) = " << tree.getMax()(1.0) << "\n";
                        else std::cout << "Max: " << tree.getMax() << "\n";
                        break;
                    }
                    case 5: { // remove
                        int key = GetInt("Key: ");
                        std::cout << (tree.remove(key) ? "Removed.\n" : "Key not found.\n");
                        break;
                    }
                    case 6: { // travarse klp
                        std::cout << "KLP traversal:\n";
                        if constexpr (std::is_same_v<T, std::function<double(double)>>)
                            tree.traverseKLP([](const T& f) { std::cout << "f(1.0)=" << f(1.0) << " "; });
                        else tree.traverseKLP([](const T& x) { std::cout << x << " "; });
                        std::cout << "\n";
                        break;
                    }
                    case 7: { // merge
                        std::cout << "Available tree indices: ";
                        for (size_t i = 0; i < globalTrees.size(); ++i)
                            std::cout << i << " ";
                        std::cout << "\n";
                        int idx = GetInt("Index of tree to merge with: ");
                        if (idx < 0 || static_cast<size_t>(idx) >= globalTrees.size()) throw Errors::IndexOutOfRange();
                        auto* other = dynamic_cast<TreeWrapper<T>*>(globalTrees[idx]);
                        if (!other) throw Errors::ConcatTypeMismatchError();
                        auto* result = new TreeWrapper<T>("merged_" + typeName);
                        result->tree = this->tree.merge(other->tree);
                        globalTrees.push_back(result);
                        typeRegistry.push_back(typeName);
                        std::cout << "Merged tree added as index " << globalTrees.size() - 1 << "\n";
                        break;
                    }
                    case 8: { // subtree
                        int key = GetInt("Key for subtree root: ");
                        auto* subtree = new TreeWrapper<T>("subtree_" + typeName);
                        subtree->tree = this->tree.extractSubtree(key);
                        globalTrees.push_back(subtree);
                        typeRegistry.push_back(typeName);
                        std::cout << "Subtree added as index " << globalTrees.size() - 1 << "\n";
                        break;
                    }
                    case 9: {
                        tree.balance();
                        std::cout << "Tree balanced.\n";
                        break;
                    }
                    case 10: return;

                    default: std::cout << "Invalid option.\n";
                }
            } catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
        }
    }
};

void ShowTypeMenu() {
    std::cout << "Choose data type:\n"
              << "1. int\n2. double\n3. string\n4. complex<double>\n"
              << "5. function<double(double)>\n6. Student\n7. Teacher\nChoice: ";
}

void RunApplication() {
    std::vector<ITreeWrapper*> trees;
    std::vector<std::string> treeTypes;

    while (true) {
        std::cout << "\n=== Main Menu ===\n"
                  << "1. Add Tree\n2. List Trees\n3. Work With Tree\n4. Delete Tree\n5. Exit\nChoose: ";
        try {
            int ch = GetInt();
            switch (ch) {
                case 1: { // Добавление нового дерева
                    ShowTypeMenu();
                    int t = GetInt();
                    switch (t) {
                        case 1: trees.push_back(new TreeWrapper<int>("int")); break;
                        case 2: trees.push_back(new TreeWrapper<double>("double")); break;
                        case 3: trees.push_back(new TreeWrapper<std::string>("string")); break;
                        case 4: trees.push_back(new TreeWrapper<std::complex<double>>("complex")); break;
                        case 5: trees.push_back(new TreeWrapper<std::function<double(double)>>("function")); break;
                        case 6: trees.push_back(new TreeWrapper<Student>("Student")); break;
                        case 7: trees.push_back(new TreeWrapper<Teacher>("Teacher")); break;
                        default: throw Errors::InvalidArgument();
                    }
                    treeTypes.push_back(trees.back()->TypeName());
                    std::cout << "Tree created. Index: " << trees.size() - 1 << "\n";
                    break;
                }
                case 2: { // Список деревьев
                    if (trees.empty()) {
                        std::cout << "No trees yet.\n";
                        break;
                    }
                    for (size_t i = 0; i < trees.size(); ++i)
                        std::cout << i << ": Tree<" << trees[i]->TypeName() << ">\n";
                    break;
                }
                case 3: { // Работа с деревом
                    if (trees.empty()) {
                        std::cout << "No trees yet.\n";
                        break;
                    }
                    std::cout << "Available tree indices: ";
                    for (size_t i = 0; i < trees.size(); ++i) std::cout << i << " ";
                    std::cout << "\n";
                    int idx = GetInt("Tree index: ");
                    if (idx < 0 || static_cast<size_t>(idx) >= trees.size()) throw Errors::IndexOutOfRange();
                    trees[idx]->Menu(trees, treeTypes);
                    break;
                }
                case 4: { // Удаление дерева
                    if (trees.empty()) {
                        std::cout << "No trees yet.\n";
                        break;
                    }
                    std::cout << "Available tree indices: ";
                    for (size_t i = 0; i < trees.size(); ++i) std::cout << i << " ";
                    std::cout << "\n";
                    int idx = GetInt("Index to remove: ");
                    if (idx < 0 || static_cast<size_t>(idx) >= trees.size()) throw Errors::IndexOutOfRange();
                    delete trees[idx];
                    trees.erase(trees.begin() + idx);
                    treeTypes.erase(treeTypes.begin() + idx);
                    std::cout << "Tree deleted.\n";
                    break;
                }
                case 5: { // Выход
                    for (auto* ptr : trees) delete ptr;
                    std::cout << "Bye!\n";
                    return;
                }
                default: std::cout << "Unknown option.\n";
            }
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }
}

int main() {
    RunApplication();
    return 0;
}
