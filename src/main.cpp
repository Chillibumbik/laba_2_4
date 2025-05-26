
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

// базовый интерфейс для обёрток деревьев
struct ITreeWrapper {
    virtual ~ITreeWrapper() = default;
};

template<typename T>
class TreeWrapper : public ITreeWrapper {
public:
    BinaryTree<T> tree;
    std::string typeName;

    TreeWrapper(std::string typeName_) : typeName(std::move(typeName_)) {}

    void Menu(std::vector<ITreeWrapper*>& globalTrees, std::vector<std::string>& typeRegistry) {
        while (true) {
            std::cout << "\n--- Tree Menu (" << typeName << ") ---\n"
                      << "1. Insert\n2. Search\n3. Min\n4. Max\n5. Remove\n"
                      << "6. Traverse (KLP)\n7. Merge with another\n8. Extract Subtree\n9. Back\n"
                      << "Available tree indices: ";
            for (size_t i = 0; i < globalTrees.size(); ++i)
                std::cout << i << " ";
            std::cout << "\nChoose: ";

            try {
                int ch = GetInt();
                switch (ch) {
                    case 1: {
                        int key = GetInt("Key: ");
                        T val = GetTyped<T>("Value: ");
                        tree.insert(key, val);
                        break;
                    }
                    case 2: {
                        int key = GetInt("Key: ");
                        T* found = tree.search(key);
                        if (found) {
                            if constexpr (std::is_same_v<T, std::function<double(double)>>)
                                std::cout << "Found: f(1.0) = " << (*found)(1.0) << "\n";
                            else std::cout << "Found: " << *found << "\n";
                        } else std::cout << "Not found\n";
                        break;
                    }
                    case 3: {
                        if constexpr (std::is_same_v<T, std::function<double(double)>>)
                            std::cout << "Min: f(1.0) = " << tree.getMin()(1.0) << "\n";
                        else std::cout << "Min: " << tree.getMin() << "\n";
                        break;
                    }
                    case 4: {
                        if constexpr (std::is_same_v<T, std::function<double(double)>>)
                            std::cout << "Max: f(1.0) = " << tree.getMax()(1.0) << "\n";
                        else std::cout << "Max: " << tree.getMax() << "\n";
                        break;
                    }
                    case 5: {
                        int key = GetInt("Key: ");
                        std::cout << (tree.remove(key) ? "Removed.\n" : "Key not found.\n");
                        break;
                    }
                    case 6: {
                        std::cout << "KLP traversal:\n";
                        if constexpr (std::is_same_v<T, std::function<double(double)>>)
                            tree.traverseKLP([](const T& f) { std::cout << "f(1.0)=" << f(1.0) << " "; });
                        else tree.traverseKLP([](const T& x) { std::cout << x << " "; });
                        std::cout << "\n";
                        break;
                    }
                    case 7: {
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
                    case 8: {
                        int key = GetInt("Key for subtree root: ");
                        auto* subtree = new TreeWrapper<T>("subtree_" + typeName);
                        subtree->tree = this->tree.extractSubtree(key);
                        globalTrees.push_back(subtree);
                        typeRegistry.push_back(typeName);
                        std::cout << "Subtree added as index " << globalTrees.size() - 1 << "\n";
                        break;
                    }
                    case 9: return;
                    default: std::cout << "Invalid option.\n";
                }
            } catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
        }
    }
};

enum class DataType {
    INT = 1, DOUBLE, STRING, COMPLEX, FUNCTION, STUDENT, TEACHER
};

void ShowTypeMenu() {
    std::cout << "Choose data type:\n"
              << "1. int\n2. double\n3. string\n4. complex<double>\n5. function<double(double)>\n6. Student\n7. Teacher\nChoice: ";
}

std::string GetTypeString(int typeCode) {
    switch (static_cast<DataType>(typeCode)) {
        case DataType::INT: return "int";
        case DataType::DOUBLE: return "double";
        case DataType::STRING: return "string";
        case DataType::COMPLEX: return "complex";
        case DataType::FUNCTION: return "function<double(double)>";
        case DataType::STUDENT: return "Student";
        case DataType::TEACHER: return "Teacher";
        default: return "unknown";
    }
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
                case 1: {
                    ShowTypeMenu();
                    int t = GetInt();
                    switch (static_cast<DataType>(t)) {
                        case DataType::INT:     trees.push_back(new TreeWrapper<int>("int")); break;
                        case DataType::DOUBLE:  trees.push_back(new TreeWrapper<double>("double")); break;
                        case DataType::STRING:  trees.push_back(new TreeWrapper<std::string>("string")); break;
                        case DataType::COMPLEX: trees.push_back(new TreeWrapper<std::complex<double>>("complex")); break;
                        case DataType::FUNCTION:trees.push_back(new TreeWrapper<std::function<double(double)>>("function")); break;
                        case DataType::STUDENT: trees.push_back(new TreeWrapper<Student>("student")); break;
                        case DataType::TEACHER: trees.push_back(new TreeWrapper<Teacher>("teacher")); break;
                        default: throw Errors::InvalidArgument();
                    }
                    treeTypes.push_back(std::to_string(t));
                    std::cout << "Tree created. Index: " << trees.size() - 1 << "\n";
                    break;
                }
                case 2: {
                    if (trees.empty()) {
                        std::cout << "No trees yet.\n";
                        break;
                    }
                    for (size_t i = 0; i < trees.size(); ++i)
                        std::cout << i << ": Tree<" << GetTypeString(std::stoi(treeTypes[i])) << ">\n";
                    break;
                }
                case 3: {
                    if (trees.empty()) {
                        std::cout << "No trees yet.\n";
                        break;
                    }
                    int idx = GetInt("Tree index: ");
                    if (idx < 0 || static_cast<size_t>(idx) >= trees.size()) throw Errors::IndexOutOfRange();
                    int t = std::stoi(treeTypes[idx]);
                    switch (static_cast<DataType>(t)) {
                        case DataType::INT:     static_cast<TreeWrapper<int>*>(trees[idx])->Menu(trees, treeTypes); break;
                        case DataType::DOUBLE:  static_cast<TreeWrapper<double>*>(trees[idx])->Menu(trees, treeTypes); break;
                        case DataType::STRING:  static_cast<TreeWrapper<std::string>*>(trees[idx])->Menu(trees, treeTypes); break;
                        case DataType::COMPLEX: static_cast<TreeWrapper<std::complex<double>>*>(trees[idx])->Menu(trees, treeTypes); break;
                        case DataType::FUNCTION:static_cast<TreeWrapper<std::function<double(double)>>*>(trees[idx])->Menu(trees, treeTypes); break;
                        case DataType::STUDENT: static_cast<TreeWrapper<Student>*>(trees[idx])->Menu(trees, treeTypes); break;
                        case DataType::TEACHER: static_cast<TreeWrapper<Teacher>*>(trees[idx])->Menu(trees, treeTypes); break;
                    }
                    break;
                }
                case 4: {
                    int idx = GetInt("Index to remove: ");
                    if (idx < 0 || static_cast<size_t>(idx) >= trees.size()) throw Errors::IndexOutOfRange();
                    delete trees[idx];
                    trees.erase(trees.begin() + idx);
                    treeTypes.erase(treeTypes.begin() + idx);
                    std::cout << "Tree deleted.\n";
                    break;
                }
                case 5: {
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
