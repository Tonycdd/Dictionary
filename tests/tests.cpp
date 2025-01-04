#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include "../src/KHeap.hpp"
#include "../src/MyVector.hpp"
#include "../src/Trie.hpp"



// Тест за конструкторите
TEST_CASE("MyVector - Default constructor creates empty vector") {
    MyVector<int> vec;
    REQUIRE(vec.getSize() == 0);
    REQUIRE(vec.getCapacity() == 0);
}

// Тест за push_back
TEST_CASE("MyVector - push_back adds elements") {
    MyVector<int> vec;
    vec.push_back(10);
    vec.push_back(20);

    REQUIRE(vec.getSize() == 2);
    REQUIRE(vec.getCapacity() >= 2);
    REQUIRE(vec[0] == 10);
    REQUIRE(vec[1] == 20);
}

// Тест за push_front
TEST_CASE("MyVector - push_front adds elements at the beginning") {
    MyVector<int> vec;
    vec.push_front(30);
    vec.push_front(40);

    REQUIRE(vec.getSize() == 2);
    REQUIRE(vec[0] == 40);
    REQUIRE(vec[1] == 30);
}

// Тест за pop_back
TEST_CASE("MyVector - pop_back removes elements from the end") {
    MyVector<int> vec;
    vec.push_back(10);
    vec.push_back(20);
    vec.pop_back();

    REQUIRE(vec.getSize() == 1);
    REQUIRE(vec[0] == 10);
}

// Тест за pop_front
TEST_CASE("MyVector - pop_front removes elements from the beginning") {
    MyVector<int> vec;
    vec.push_back(10);
    vec.push_back(20);
    vec.pop_front();

    REQUIRE(vec.getSize() == 1);
    REQUIRE(vec[0] == 20);
}

// Тест за resize
TEST_CASE("MyVector - resizing doubles capacity when full") {
    MyVector<int> vec;
    for (int i = 0; i < 10; ++i) {
        vec.push_back(i);
    }

    REQUIRE(vec.getSize() == 10);
    REQUIRE(vec.getCapacity() >= 10);
    REQUIRE(vec[9] == 9);
}

// Тест за оператор []
TEST_CASE("MyVector - operator[] gives access to elements") {
    MyVector<int> vec;
    vec.push_back(100);
    vec.push_back(200);

    REQUIRE(vec[0] == 100);
    REQUIRE(vec[1] == 200);
}

// Тест за копиращ конструктор
TEST_CASE("MyVector - copy constructor creates independent copy") {
    MyVector<int> vec1;
    vec1.push_back(10);
    vec1.push_back(20);

    MyVector<int> vec2 = vec1;
    vec2.push_back(30);

    REQUIRE(vec1.getSize() == 2);
    REQUIRE(vec2.getSize() == 3);
}

// Тест за оператор =
TEST_CASE("MyVector - assignment operator works correctly") {
    MyVector<int> vec1;
    vec1.push_back(10);
    vec1.push_back(20);

    MyVector<int> vec2;
    vec2 = vec1;

    REQUIRE(vec2.getSize() == 2);
    REQUIRE(vec2[0] == 10);
    REQUIRE(vec2[1] == 20);
}


TEST_CASE("Testing TrieDictionary basic functionalities", "[TrieDictionary]") {

    SECTION("Insert and Search") {
        TrieDictionary dict;
        dict.insert("apple", 5);
        dict.insert("app", 3);
        dict.insert("banana", 7);

        REQUIRE(dict.search("apple", 5));
        REQUIRE(dict.search("app", 3));
        REQUIRE(dict.search("banana", 7));
        REQUIRE(!dict.search("banana", 5)); // Wrong score
        REQUIRE(!dict.search("orange", 4)); // Non-existent word
    }

    SECTION("_search returns correct scores") {
        TrieDictionary dict;
        dict.insert("apple", 5);
        dict.insert("app", 3);

        REQUIRE(dict._search("apple") == 5);
        REQUIRE(dict._search("app") == 3);
        REQUIRE(dict._search("banana") == -1); // Non-existent word
    }

    SECTION("Display content") {
        TrieDictionary dict;
        dict.insert("cat", 1);
        dict.insert("car", 2);
        dict.insert("cart", 3);

        // The display function is visual; we can't directly assert its output.
        // But we can verify manually if needed.
        dict.display();
    }

    SECTION("Build from file") {
        std::ofstream file("test_file.txt");
        file << "apple 5\napp 3\nbanana 7\ninvalid_word -1\n";
        file.close();

        TrieDictionary dict("test_file.txt");

        REQUIRE(dict.search("apple", 5));
        REQUIRE(dict.search("app", 3));
        REQUIRE(dict.search("banana", 7));
        REQUIRE(!dict.search("invalid_word", -1)); // Invalid word skipped
    }

    SECTION("Copy constructor and assignment operator") {
        TrieDictionary dict1;
        dict1.insert("hello", 10);
        dict1.insert("world", 20);

        TrieDictionary dict2(dict1); // Copy constructor
        REQUIRE(dict2.search("hello", 10));
        REQUIRE(dict2.search("world", 20));

        TrieDictionary dict3;
        dict3 = dict1; // Assignment operator
        REQUIRE(dict3.search("hello", 10));
        REQUIRE(dict3.search("world", 20));
    }
}


TEST_CASE("KHeap basic operations") {
    MyVector<std::pair<int, std::string>> elements;
    elements.push_back({10, "apple"});
    elements.push_back({20, "banana"});
    elements.push_back({5, "cherry"});
    elements.push_back({15, "date"});

    SECTION("Heap construction with k=2 (binary heap)") {
        KHeap heap(2, elements);
        REQUIRE(heap.getSize() == 4);
        REQUIRE(heap.getMin().first == 20); // Максимална пирамида
    }

    SECTION("Heap construction with k=3 (ternary heap)") {
        KHeap heap(3, elements);
        REQUIRE(heap.getSize() == 4);
        REQUIRE(heap.getMin().first == 20);
    }

    SECTION("Invalid k throws exception") {
        REQUIRE_THROWS_AS(KHeap(1, elements), std::invalid_argument);
    }

    SECTION("Insert operation") {
        KHeap heap(2, elements);
        heap.insert(25, "elderberry");
        REQUIRE(heap.getSize() == 5);
        REQUIRE(heap.getMin().first == 25);
    }

    SECTION("Extract max element") {
        KHeap heap(2, elements);
        auto maxElement = heap.extract();
        REQUIRE(maxElement.first == 20);
        REQUIRE(maxElement.second == "banana");
        REQUIRE(heap.getSize() == 3);
    }

    SECTION("Extract from empty heap throws exception") {
        MyVector<std::pair<int, std::string>> emptyElements;
        KHeap heap(2, emptyElements);
        REQUIRE(heap.empty() == true);
        REQUIRE_THROWS_AS(heap.extract(), std::logic_error);
    }

    SECTION("Heap after multiple insert and extract operations") {
        KHeap heap(3, elements);

        heap.insert(25, "fig");
        heap.insert(30, "grape");
        REQUIRE(heap.getSize() == 6);
        REQUIRE(heap.getMin().first == 30);

        auto maxElement = heap.extract();
        REQUIRE(maxElement.first == 30);
        REQUIRE(maxElement.second == "grape");
        REQUIRE(heap.getSize() == 5);

        maxElement = heap.extract();
        REQUIRE(maxElement.first == 25);
        REQUIRE(maxElement.second == "fig");
        REQUIRE(heap.getSize() == 4);
    }

    SECTION("Print heap structure") {
        KHeap heap(2, elements);
        heap.print(); // Визуално проверяваме резултата, ако е нужно
    }
}
