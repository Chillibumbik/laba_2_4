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

template<typename T>
class TreeWrapper {
private:
    BinaryTree<T> tree;
    std::string typeName;

public:
    TreeWrapper(std::string typeName_) : typeName(std::move(typeName_)) {}

    std::string Info() const {
        return "BinaryTree<" + typeName + ">";
    }

    void Menu() {
        while (true) {
            std::cout << "\n--- Tree Menu (" << typeName << ") ---\n"
                      << "1. Insert\n2. Search\n3. Min\n4. Max\n5. Remove\n"
                      << "6. Traverse (KLP)\n7. Back\nChoose: ";
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
                            if constexpr (std::is_same_v<T, std::function<double(double)>>) {
                                std::cout << "Found: f(1.0) = " << (*found)(1.0) << "\n";
                            } else {
                                std::cout << "Found: " << *found << "\n";
                            }
                        } else {
                            std::cout << "Not found\n";
                        }
                        break;
                    }
                    case 3: { // getMin
                        if constexpr (std::is_same_v<T, std::function<double(double)>>) {
                            std::cout << "Min: f(1.0) = " << tree.getMin()(1.0) << "\n";
                        } else {
                            std::cout << "Min: " << tree.getMin() << "\n";
                        }
                        break;
                    }
                    case 4: { // getMax
                        if constexpr (std::is_same_v<T, std::function<double(double)>>) {
                            std::cout << "Max: f(1.0) = " << tree.getMax()(1.0) << "\n";
                        } else {
                            std::cout << "Max: " << tree.getMax() << "\n";
                        }
                        break;
                    }
                    case 5: { // remove
                        int key = GetInt("Key: ");
                        if (tree.remove(key)) std::cout << "Removed.\n";
                        else std::cout << "Key not found.\n";
                        break;
                    }
                    case 6: { // traverse
                        if constexpr (std::is_same_v<T, std::function<double(double)>>) {
                            tree.traverseKLP([](const T& f) {
                                std::cout << "f(1.0)=" << f(1.0) << " ";
                            });
                        } else {
                            tree.traverseKLP([](const T& x) {
                                std::cout << x << " ";
                            });
                        }
                        std::cout << "\n";
                        break;
                    }
                    case 7: return; // back
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

void RunApplication() {
    std::vector<void*> trees;
    std::vector<std::string> treeTypes;

    while (true) {
        std::cout << "\n=== Main Menu ===\n"
                  << "1. Add Tree\n"
                  << "2. List Trees\n"
                  << "3. Work With Tree\n"
                  << "4. Delete Tree\n"
                  << "5. Exit\nChoose: ";

        try {
            int ch = GetInt();
            switch (ch) {
                case 1: { // Add tree
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

                case 2: { // List trees
                    if (trees.empty()) {
                        std::cout << "No trees yet.\n";
                        break;
                    }
                    for (size_t i = 0; i < trees.size(); ++i)
                        std::cout << i << ": Tree<" << treeTypes[i] << ">\n";
                    break;
                }

                case 3: { // Work with tree
                    if (trees.empty()) {
                        std::cout << "No trees yet.\n";
                        break;
                    }
                    int idx = GetInt("Tree index: ");
                    if (idx < 0 || static_cast<size_t>(idx) >= trees.size()) throw Errors::IndexOutOfRange();

                    int t = std::stoi(treeTypes[idx]);
                    switch (static_cast<DataType>(t)) {
                        case DataType::INT:     static_cast<TreeWrapper<int>*>(trees[idx])->Menu(); break;
                        case DataType::DOUBLE:  static_cast<TreeWrapper<double>*>(trees[idx])->Menu(); break;
                        case DataType::STRING:  static_cast<TreeWrapper<std::string>*>(trees[idx])->Menu(); break;
                        case DataType::COMPLEX: static_cast<TreeWrapper<std::complex<double>>*>(trees[idx])->Menu(); break;
                        case DataType::FUNCTION:static_cast<TreeWrapper<std::function<double(double)>>*>(trees[idx])->Menu(); break;
                        case DataType::STUDENT: static_cast<TreeWrapper<Student>*>(trees[idx])->Menu(); break;
                        case DataType::TEACHER: static_cast<TreeWrapper<Teacher>*>(trees[idx])->Menu(); break;
                    }
                    break;
                }

                case 4: { // Delete tree
                    int idx = GetInt("Index to remove: ");
                    if (idx < 0 || static_cast<size_t>(idx) >= trees.size()) throw Errors::IndexOutOfRange();
                    int t = std::stoi(treeTypes[idx]);
                    switch (static_cast<DataType>(t)) {
                        case DataType::INT:     delete static_cast<TreeWrapper<int>*>(trees[idx]); break;
                        case DataType::DOUBLE:  delete static_cast<TreeWrapper<double>*>(trees[idx]); break;
                        case DataType::STRING:  delete static_cast<TreeWrapper<std::string>*>(trees[idx]); break;
                        case DataType::COMPLEX: delete static_cast<TreeWrapper<std::complex<double>>*>(trees[idx]); break;
                        case DataType::FUNCTION:delete static_cast<TreeWrapper<std::function<double(double)>>*>(trees[idx]); break;
                        case DataType::STUDENT: delete static_cast<TreeWrapper<Student>*>(trees[idx]); break;
                        case DataType::TEACHER: delete static_cast<TreeWrapper<Teacher>*>(trees[idx]); break;
                    }
                    trees.erase(trees.begin() + idx);
                    treeTypes.erase(treeTypes.begin() + idx);
                    std::cout << "Tree deleted.\n";
                    break;
                }

                case 5: { // Exit
                    for (size_t i = 0; i < trees.size(); ++i) {
                        int t = std::stoi(treeTypes[i]);
                        switch (static_cast<DataType>(t)) {
                            case DataType::INT:     delete static_cast<TreeWrapper<int>*>(trees[i]); break;
                            case DataType::DOUBLE:  delete static_cast<TreeWrapper<double>*>(trees[i]); break;
                            case DataType::STRING:  delete static_cast<TreeWrapper<std::string>*>(trees[i]); break;
                            case DataType::COMPLEX: delete static_cast<TreeWrapper<std::complex<double>>*>(trees[i]); break;
                            case DataType::FUNCTION:delete static_cast<TreeWrapper<std::function<double(double)>>*>(trees[i]); break;
                            case DataType::STUDENT: delete static_cast<TreeWrapper<Student>*>(trees[i]); break;
                            case DataType::TEACHER: delete static_cast<TreeWrapper<Teacher>*>(trees[i]); break;
                        }
                    }
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
