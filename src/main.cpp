 // Kendi yapimizi dahil ettik
#include <iostream>
#include <string>
#include "../include/data_structures/HashMap.h"
#include "../include/data_structures/LinkedList.h"

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

//HashMap test
  std::cout << "\n[TEST] HashMap Calisiyor mu?" << std::endl;
  HashMap<std::string, int> studentGrades;
  studentGrades.insert("Alice", 85);
  studentGrades.insert("Bob", 92);

  std::cout << "Alice'in notu: " << *studentGrades.get("Alice") << std::endl;
  std::cout << "Bob'un notu: " << *studentGrades.get("Bob") << std::endl;  
  std::cout << "[TEST] Basarili!" << std::endl;

  

  return 0;
}