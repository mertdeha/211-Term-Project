/**
 * @file test_scheduler_pipeline.cpp
 * @brief Sınav çizelgeleme entegrasyon süreçlerini test eden ana modül.
 */

#include <cassert>
#include <iostream>

#include "core/Course.h"
#include "core/Scheduler.h"
#include "data_structures/DynamicArray.h"
#include "data_structures/Graph.h"
#include "data_structures/HashMap.h"

#ifndef nullptr
#define nullptr NULL
#endif

/**
 * @brief Senaryo 1: Temel Çakışma ve Renklendirme Entegrasyonu
 * İki çakışan ve bir bağımsız ders ile pipeline'ı doğrular.
 */
void testBasicConflictPipeline() {
    std::cout << "[INTEGRATION TEST] Running: Basic Conflict Pipeline...\n";

    DynamicArray<Course *> courses;
    Course *c1 = new Course("IE301", "OR");
    c1->addStudent("StudentA");
    c1->addStudent("StudentB");
    Course *c2 = new Course("IE302", "Sim");
    c2->addStudent("StudentB");  // Çakışma
    Course *c3 = new Course("MATH201", "LA");
    c3->addStudent("StudentC");

    courses.push_back(c1);
    courses.push_back(c2);
    courses.push_back(c3);

    Graph conflictGraph;
    Scheduler::buildConflictGraph(courses, conflictGraph);

    HashMap<std::string, int> courseToSlotMap;
    bool success = Scheduler::colorGraph(courses, conflictGraph, 3, courseToSlotMap);

    assert(success == true);
    assert(*courseToSlotMap.get("IE301") != *courseToSlotMap.get("IE302"));

    for (int i = 0; i < courses.size(); i++) delete courses.get(i);
    std::cout << "[PASS] Basic pipeline verified.\n";
}

/**
 * @brief Senaryo 2: "Çizelgelenemez" Durum Testi
 * Çok az zaman dilimi (slot) verilirse sistemin başarısız olduğunu (false) doğrular.
 */
void testImpossibleScheduling() {
    std::cout << "[INTEGRATION TEST] Running: Impossible Scheduling Scenario...\n";

    DynamicArray<Course *> courses;
    courses.push_back(new Course("C1", "D1"));
    courses.push_back(new Course("C2", "D2"));

    // Dersleri birbirine bağla (Çakışma var)
    Graph conflictGraph;
    conflictGraph.addVertex("C1");
    conflictGraph.addVertex("C2");
    conflictGraph.addEdge("C1", "C2");

    HashMap<std::string, int> courseToSlotMap;
    // 2 ders var ama sadece 1 slot veriyoruz -> Boyanamaz
    bool success = Scheduler::colorGraph(courses, conflictGraph, 1, courseToSlotMap);

    assert(success == false);

    for (int i = 0; i < courses.size(); i++) delete courses.get(i);
    std::cout << "[PASS] Impossible scheduling correctly flagged.\n";
}

/**
 * @brief Entegrasyon test paketini yöneten ana giriş noktası.
 */
int main() {
    testBasicConflictPipeline();
    testImpossibleScheduling();

    std::cout << "\n[SUCCESS] All Integration tests passed cleanly!\n";
    return 0;
}