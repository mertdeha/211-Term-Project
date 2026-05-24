#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "../data_structures/DynamicArray.h"
#include "../data_structures/Graph.h"
#include "Course.h"

/**
 * @brief Sinavlarin cakisimlarini hesaplayan ve zaman atamalarini yapan sinif.
 */
class Scheduler {
public:
  /**
   * @brief Verilen ders listesindeki ogrencileri karsilastirarak Cakisim
   * Grafini kurar.
   * @param allCourses JSON'dan okunmus tum derslerin listesi
   * @param conflictGraph Uzerine cizgilerin (Edge) eklenecegi bos graf nesnesi
   */

  // ÇAKIŞMA GRAFINI OLUŞTURAN FONKSİYON

  static void buildConflictGraph(DynamicArray<Course *> &allCourses,
                                 Graph &conflictGraph) {

    // 1. Tum dersleri grafa dugum (Vertex) olarak ekle
    for (int i = 0; i < allCourses.size(); i++) {
      conflictGraph.addVertex(allCourses.get(i)->id);
    }

    // 2. Dersleri birbiriyle karsilastir (Ortak ogrenci var mi?)
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
  }

  // İleride buraya: static void colorGraph(...) fonksiyonunu ekleyecegiz!
};

#endif // SCHEDULER_H