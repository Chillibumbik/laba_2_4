#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "BinaryTree.hpp"
#include "Users.hpp"
#include "Errors.hpp"
#include <complex>
#include <fstream>
#include <chrono>
#include <cmath>
#include <random>
#include <numeric>



TEST_CASE("BinaryTree<int>: Insert, Search, Min/Max, Remove") {
    BinaryTree<int> tree;
    tree.insert(10, 100);
    tree.insert(5, 50);
    tree.insert(15, 150);

    SECTION("Search existing keys") {
        REQUIRE(*tree.search(10) == 100);
        REQUIRE(*tree.search(5) == 50);
        REQUIRE(*tree.search(15) == 150);
    }

    SECTION("Search non-existing key") {
        REQUIRE(tree.search(42) == nullptr);
    }

    SECTION("Min/Max") {
        REQUIRE(tree.getMin() == 50);
        REQUIRE(tree.getMax() == 150);
    }

    SECTION("Remove") {
        REQUIRE(tree.remove(5));
        REQUIRE(tree.search(5) == nullptr);
        REQUIRE_FALSE(tree.remove(5));
    }
}

TEST_CASE("BinaryTree<User>: Insert and Search") {
    BinaryTree<User> tree;
    User u1{"Alice", 30, 1};
    User u2{"Bob", 25, 2};

    tree.insert(1, u1);
    tree.insert(2, u2);

    REQUIRE(*tree.search(1) == u1);
    REQUIRE(*tree.search(2) == u2);
    REQUIRE(tree.search(3) == nullptr);
}

TEST_CASE("BinaryTree: Merge") {
    BinaryTree<int> a, b;
    a.insert(1, 10);
    a.insert(2, 20);
    b.insert(3, 30);

    BinaryTree<int> c = a.merge(b);
    REQUIRE(*c.search(1) == 10);
    REQUIRE(*c.search(2) == 20);
    REQUIRE(*c.search(3) == 30);
}

TEST_CASE("BinaryTree: Extract Subtree") {
    BinaryTree<int> tree;
    tree.insert(10, 100);
    tree.insert(5, 50);
    tree.insert(15, 150);
    tree.insert(12, 120);

    BinaryTree<int> sub = tree.extractSubtree(15);
    REQUIRE(sub.search(15));
    REQUIRE(sub.search(12));
    REQUIRE_FALSE(sub.search(10));
    REQUIRE_FALSE(sub.search(5));
}

TEST_CASE("BinaryTree: Traverse KLP") {
    BinaryTree<int> tree;
    tree.insert(10, 1);
    tree.insert(5, 2);
    tree.insert(15, 3);

    std::vector<int> values;
    tree.traverseKLP([&](const int& val) { values.push_back(val); });
    REQUIRE(values == std::vector<int>{1, 2, 3});
}

TEST_CASE("BinaryTree: Exception Cases") {
    BinaryTree<int> empty;
    REQUIRE_THROWS_AS(empty.getMin(), std::runtime_error);
    REQUIRE_THROWS_AS(empty.getMax(), std::runtime_error);
    REQUIRE_FALSE(empty.remove(999));
}

TEST_CASE("BinaryTree<double>: Precision test") {
    BinaryTree<double> tree;
    tree.insert(1, 3.1415);
    tree.insert(2, 2.7182);
    tree.insert(3, 3.12);
    tree.insert(5, 32.11);

    REQUIRE(tree.getMin() == Approx(3.1415));
    REQUIRE(tree.getMax() == Approx(32.11));
}

TEST_CASE("BinaryTree<std::string>: Search and Comparison") {
    BinaryTree<std::string> tree;
    tree.insert(5, "five");
    tree.insert(3, "three");
    tree.insert(7, "seven");

    REQUIRE(*tree.search(3) == "three");
    REQUIRE(*tree.search(5) == "five");
    REQUIRE(*tree.search(7) == "seven");
    REQUIRE(tree.search(9) == nullptr);
}

TEST_CASE("BinaryTree<std::complex<double>>: Complex value support") {
    using cd = std::complex<double>;
    BinaryTree<cd> tree;
    tree.insert(1, cd(1.0, 2.0));
    tree.insert(2, cd(3.0, 4.0));

    REQUIRE(tree.search(1)->real() == Approx(1.0));
    REQUIRE(tree.search(1)->imag() == Approx(2.0));
    REQUIRE(tree.search(2)->real() == Approx(3.0));
}

TEST_CASE("BinaryTree: Traversal empty tree") {
    BinaryTree<int> tree;
    std::vector<int> values;
    tree.traverseKLP([&](const int& val) { values.push_back(val); });
    REQUIRE(values.empty());
}

TEST_CASE("BinaryTree: Deep Merge and Extract") {
    BinaryTree<int> tree1, tree2;
    for (int i = 0; i < 5; ++i)
        tree1.insert(i, i * 10);
    for (int i = 5; i < 10; ++i)
        tree2.insert(i, i * 10);

    BinaryTree<int> merged = tree1.merge(tree2);
    for (int i = 0; i < 10; ++i)
        REQUIRE(*merged.search(i) == i * 10);

    BinaryTree<int> subtree = merged.extractSubtree(5);
    REQUIRE(subtree.search(5));
    REQUIRE(subtree.search(6));
}


TEST_CASE("BinaryTree: Overwrite value with same key") {
    BinaryTree<int> tree;
    tree.insert(1, 100);
    tree.insert(1, 200); 
    REQUIRE(*tree.search(1) == 200);
}

TEST_CASE("BinaryTree: Large input stress test") {
    BinaryTree<int> tree;
    const int N = 1000;
    for (int i = 0; i < N; ++i) tree.insert(i, i * 2);

    for (int i = 0; i < N; i += 50)
        REQUIRE(*tree.search(i) == i * 2);

    for (int i = 0; i < N; i += 100)
        REQUIRE(tree.remove(i));

    for (int i = 0; i < N; i += 100)
        REQUIRE(tree.search(i) == nullptr);
}

TEST_CASE("BinaryTree: Remove root with two children") {
    BinaryTree<int> tree;
    tree.insert(20, 20);
    tree.insert(10, 10);
    tree.insert(30, 30);
    REQUIRE(tree.remove(20));
    REQUIRE(tree.search(20) == nullptr);
    REQUIRE(*tree.search(10) == 10);
    REQUIRE(*tree.search(30) == 30);
}

TEST_CASE("BinaryTree: Insert descending order") {
    BinaryTree<int> tree;
    for (int i = 10; i >= 1; --i) tree.insert(i, i);
    for (int i = 1; i <= 10; ++i) REQUIRE(*tree.search(i) == i);
}  

TEST_CASE("BinaryTree: Insert ascending order") {
    BinaryTree<int> tree;
    for (int i = 1; i <= 10; ++i) tree.insert(i, i);
    for (int i = 1; i <= 10; ++i) REQUIRE(*tree.search(i) == i);
}

TEST_CASE("BinaryTree: Manual traversal check") {
    BinaryTree<int> tree;
    tree.insert(10, 100);
    tree.insert(5, 50);
    tree.insert(15, 150);

    std::vector<int> visited;
    tree.traverseKLP([&](const int& val) {
        visited.push_back(val);
    });

    REQUIRE(visited.size() == 3);
    REQUIRE(visited[0] == 100); // к
    REQUIRE(visited[1] == 50);  // л
    REQUIRE(visited[2] == 150); // п
}  

TEST_CASE("BinaryTree: Balance reduces tree depth") {
    BinaryTree<int> tree;

    // перекошено вправо
    for (int i = 1; i <= 7; ++i) {
        tree.insert(i, i * 10);
    }

    int depthBefore = tree.GetDepth();
    REQUIRE(depthBefore == 7); 

    tree.balance();

    int depthAfter = tree.GetDepth();
    REQUIRE(depthAfter < depthBefore);
    REQUIRE(depthAfter <= 3); // log2(7+1) = 3

    // Данные остались
    for (int i = 1; i <= 7; ++i) {
        REQUIRE(*tree.search(i) == i * 10);
    }
}



void benchmark_binary_tree(const std::string& filename) {
    std::ofstream file(filename);
    file << "N,InsertTimeMs,SearchTimeMs\n";

    for (int exp = 1; exp <= 7; ++exp) { 
        size_t N = static_cast<size_t>(std::pow(10, exp));

        BinaryTree<int> tree;

        // генерация и шафл
        std::vector<int> keys(N);
        std::iota(keys.begin(), keys.end(), 0);
        std::shuffle(keys.begin(), keys.end(), std::mt19937{std::random_device{}()});

        // вставка
        auto t1 = std::chrono::high_resolution_clock::now();
        for (int key : keys) {
            tree.insert(key, key);
        }
        auto t2 = std::chrono::high_resolution_clock::now();
        double insert_time = std::chrono::duration<double, std::milli>(t2 - t1).count();

        // поиск
        t1 = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < std::min(N, size_t(1000)); ++i) {
            tree.search(keys[i]);
        }
        t2 = std::chrono::high_resolution_clock::now();
        double search_time = std::chrono::duration<double, std::milli>(t2 - t1).count();

        file << N << "," << insert_time << "," << search_time << "\n";
    }

    file.close();
}

TEST_CASE("Benchmark: BinaryTree performance", "[Benchmark]") {
    benchmark_binary_tree("benchmark_results.csv");
    REQUIRE(true);
}

TEST_CASE("BinaryTree: serialize and deserialize") {
    BinaryTree<int> tree;
    tree.insert(20, 20);
    tree.insert(10, 10);
    tree.insert(30, 30);

    std::string serialized = tree.toString();


    REQUIRE(tree.isValidTreeString(serialized));


    BinaryTree<int> new_tree = BinaryTree<int>::fromString(serialized);
    REQUIRE(new_tree == tree);


    std::string invalid_bst = "((()25:25())20:20(()30:30()))"; // 25 > 20  ошибка
    BinaryTree<int> tmp_tree;
    REQUIRE_FALSE(tmp_tree.isValidTreeString(invalid_bst));
}
