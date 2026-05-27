/**
 * @file test_data_structures.cpp
 * @brief Veri yapılarının (LinkedList, HashMap, DynamicArray, Graph) birim testleri.
 */

#include <cassert>
#include <iostream>
#include <string>

#include "../../include/data_structures/DynamicArray.h"
#include "../../include/data_structures/Graph.h"
#include "../../include/data_structures/HashMap.h"
#include "../../include/data_structures/LinkedList.h"

#ifndef nullptr
#define nullptr NULL
#endif

/**
 * @brief LinkedList veri yapısının temel fonksiyonlarını test eder.
 */
void testLinkedList() {
    std::cout << "[UNIT TEST] Testing LinkedList..." << std::endl;
    LinkedList<std::string> list;
    list.insert("CSE101");
    list.insert("CSE201");

    assert(list.getSize() == 2);
    std::cout << "[PASS] LinkedList test successful." << std::endl;
}

/**
 * @brief HashMap veri yapısının anahtar-değer eşlemesini test eder.
 */
void testHashMap() {
    std::cout << "[UNIT TEST] Testing HashMap..." << std::endl;
    HashMap<std::string, int> map;
    map.insert("Alice", 85);

    int *val = map.get("Alice");
    assert(val != nullptr && *val == 85);
    std::cout << "[PASS] HashMap test successful." << std::endl;
}

/**
 * @brief DynamicArray veri yapısının boyut ve eleman yönetimini test eder.
 */
void testDynamicArray() {
    std::cout << "[UNIT TEST] Testing DynamicArray..." << std::endl;
    DynamicArray<std::string> arr;
    arr.push_back("Ahmet");
    arr.push_back("Ayse");

    assert(arr.size() == 2);
    assert(arr.get(0) == "Ahmet");
    std::cout << "[PASS] DynamicArray test successful." << std::endl;
}

/**
 * @brief Graph veri yapısının düğüm ve kenar (vertex/edge) mantığını test eder.
 */
void testGraph() {
    std::cout << "[UNIT TEST] Testing Graph..." << std::endl;
    Graph g;
    g.addVertex("A");
    g.addVertex("B");
    g.addEdge("A", "B");

    assert(g.getDegree("A") == 1);
    std::cout << "[PASS] Graph test successful." << std::endl;
}

/**
 * @brief Tüm birim testlerini sırasıyla çalıştıran ana fonksiyon.
 * @return int Başarı durumu (0).
 */
int main() {
    std::cout << "--- Starting Unit Tests: Data Structures ---\n" << std::endl;

    testLinkedList();
    testHashMap();
    testDynamicArray();
    testGraph();

    std::cout << "\n[SUCCESS] All unit tests completed successfully!" << std::endl;
    return 0;
}