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

  std::cout << "--- PROJ-16: Sinav Cizelgeleme Sistemi ---" << std::endl;
  std::cout << "Veri dosyasi okunuyor: " << inputFile << std::endl;

  // --- LINKEDLIST TESTI BASLIYOR ---
  std::cout << "\n[TEST] Bagli Liste (LinkedList) Calisiyor mu?" << std::endl;

  // String tutan bir liste olusturalim (Ders isimleri olsun)
  LinkedList<std::string> courseList;

  courseList.insert("CSE101");
  courseList.insert("CSE201");
  courseList.insert("MATH101");

  std::cout << "Listeye eklenen ders sayisi: " << courseList.getSize()
            << std::endl;

  // Listeyi bastan sona gezerek ekrana yazdirma (Iterasyon)
  std::cout << "Dersler: ";
  Node<std::string> *current = courseList.getHead();
  while (current != NULL) {
    std::cout << current->data;
    if (current->next != NULL)
      std::cout << " -> ";
    current = current->next;
  }
  std::cout << std::endl;
  std::cout << "[TEST] Basarili!" << std::endl;

  // HashMap test
  std::cout << "\n[TEST] HashMap Calisiyor mu?" << std::endl;
  HashMap<std::string, int> studentGrades;
  studentGrades.insert("Alice", 85);
  studentGrades.insert("Bob", 92);

  std::cout << "Alice'in notu: " << *studentGrades.get("Alice") << std::endl;
  std::cout << "Bob'un notu: " << *studentGrades.get("Bob") << std::endl;
  std::cout << "[TEST] Basarili!" << std::endl;

  // Graph test

  Graph conflictGraph;

  // 2. Dersleri (Dugumleri) grafa ekle
  conflictGraph.addVertex("CSE101");
  conflictGraph.addVertex("CSE201");
  conflictGraph.addVertex("MATH101");
  conflictGraph.addVertex("PHYS101");

  // 3. Cakismalari (Kenarlari) belirle
  // Ornegin: Ayni ogrenci hem CSE101 hem de MATH101 aliyor
  conflictGraph.addEdge("CSE101", "MATH101");
  conflictGraph.addEdge("CSE101", "PHYS101");
  conflictGraph.addEdge("CSE201", "MATH101");

  // 4. Grafi ekrana yazdir
  std::cout << "[TEST] Cakisim Grafi (Conflict Graph):\n";
  conflictGraph.printGraph();

  // --- DYNAMIC ARRAY TESTI BASLIYOR ---
  std::cout << "\n[TEST] Dinamik Dizi (DynamicArray) Calisiyor mu?"
            << std::endl;

  DynamicArray<std::string> ogrenciler;

  ogrenciler.push_back("Ahmet");
  ogrenciler.push_back("Ayse");
  ogrenciler.push_back(
      "Mehmet"); // Burada kapasite aşılacak ve resize() çalışacak

  std::cout << "Diziye eklenen ogrenci sayisi: " << ogrenciler.size()
            << std::endl;
  std::cout << "Ogrenciler: ";

  for (int i = 0; i < ogrenciler.size(); i++) {
    std::cout << ogrenciler.get(i);
    if (i != ogrenciler.size() - 1) {
      std::cout << ", ";
    }
  }
  std::cout << "\n[TEST] DynamicArray Basarili!" << std::endl;

  return 0;
}