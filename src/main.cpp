#include "../include/core/Course.h"
#include "../include/core/Parser.h"
#include "../include/core/Scheduler.h"
#include "../include/data_structures/DynamicArray.h"
#include "../include/data_structures/Graph.h"
#include "../include/data_structures/HashMap.h"
#include "../include/data_structures/LinkedList.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  std::string inputFile = "data/input_sample.json";
  if (argc > 1) {
    inputFile = argv[1];
  }

  std::cout << "--- PROJ-16: Sinav Cizelgeleme Sistemi ---\n\n";

  // 1. VERILERI OKU
  DynamicArray<Course *> allCourses;
  Parser::parseJSON(inputFile, allCourses);

  // 2. CAKISMA GRAFINI KUR (Scheduler uzerinden, tertemiz!)
  std::cout << ">>> ADIM 2: Cakisim Grafi Insa Ediliyor...\n";
  Graph conflictGraph;

  Scheduler::buildConflictGraph(allCourses,
                                conflictGraph); // Tek satirda hallettik!

  std::cout << "\n[BASARILI] Graf olusturuldu! Iste otomatik cakisim durumu:\n";
  conflictGraph.printGraph();

  // Hafiza temizligi
  for (int i = 0; i < allCourses.size(); i++) {
    delete allCourses.get(i);
  }

  return 0;
}