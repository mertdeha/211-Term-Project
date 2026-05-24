#include "../include/core/Course.h"
#include "../include/core/Parser.h"
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

  // BURASI DEGISITI: Course yerine Course* tutuyoruz
  DynamicArray<Course *> allCourses;
  Parser::parseJSON(inputFile, allCourses);

  std::cout << ">>> ADIM 2: Cakisim Grafi Insa Ediliyor...\n";
  Graph conflictGraph;

  for (int i = 0; i < allCourses.size(); i++) {
    // Pointer oldugu icin '->' ok ile erisiyoruz
    conflictGraph.addVertex(allCourses.get(i)->id);
  }

  for (int i = 0; i < allCourses.size(); i++) {
    for (int j = i + 1; j < allCourses.size(); j++) {

      bool conflictFound = false;
      Node<std::string> *studentA =
          allCourses.get(i)->enrolledStudents.getHead();

      while (studentA != NULL && !conflictFound) {
        Node<std::string> *studentB =
            allCourses.get(j)->enrolledStudents.getHead();
        while (studentB != NULL) {
          if (studentA->data == studentB->data) {
            conflictFound = true;
            break;
          }
          studentB = studentB->next;
        }
        studentA = studentA->next;
      }

      if (conflictFound) {
        conflictGraph.addEdge(allCourses.get(i)->id, allCourses.get(j)->id);
      }
    }
  }

  std::cout << "\n[BASARILI] Graf olusturuldu! Iste otomatik cakisim durumu:\n";
  conflictGraph.printGraph();

  // Hafiza temizligi (Memory Leak onlemek icin)
  for (int i = 0; i < allCourses.size(); i++) {
    delete allCourses.get(i);
  }

  return 0;
}