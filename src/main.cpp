#include "../include/core/Course.h"
#include "../include/core/Parser.h"
#include "../include/core/Scheduler.h"
#include "../include/core/Frontend.h" 
#include "../include/data_structures/DynamicArray.h"
#include "../include/data_structures/Graph.h"
#include "../include/data_structures/HashMap.h"
#include "../include/data_structures/LinkedList.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  std::cout << "=========================================================================\n";
  std::cout << "--- PROJ-16: Sinav Cizelgeleme Sistemi---\n";
  std::cout << "=========================================================================\n\n";

  DynamicArray<Course *> allCourses;

  std::string inputFile = "data/input_sample.json";
  if (argc > 1) {
    inputFile = argv[1];
  }

  Parser::parseJSON(inputFile, allCourses);

  if (allCourses.size() == 0) {
    std::cout << "[INFO] '" << inputFile << "' bulunamadi veya bos. Test verileri otomatik uretiliyor...\n";
    Course *c1 = new Course("CSE101", "Intro to CS");
    c1->addStudent("Ahmet"); c1->addStudent("Mehmet"); c1->addStudent("Can");
    
    Course *c2 = new Course("CSE102", "Data Structures"); 
    c2->addStudent("Mehmet"); c2->addStudent("Elif"); c2->addStudent("Zeynep");
    
    Course *c3 = new Course("MATH101", "Calculus I"); 
    c3->addStudent("Ahmet"); c3->addStudent("Buse"); c3->addStudent("Kaan");
    
    Course *c4 = new Course("PHYS101", "Physics I"); 
    c4->addStudent("Elif"); c4->addStudent("Kaan"); c4->addStudent("Arda");

    Course *c5 = new Course("CSE211", "Digital Design"); 
    c5->addStudent("Seda"); c5->addStudent("Umut"); c5->addStudent("Omer"); 
    c5->addStudent("Ali"); c5->addStudent("Veli"); c5->addStudent("Ayse");

    allCourses.push_back(c1);
    allCourses.push_back(c2);
    allCourses.push_back(c3);
    allCourses.push_back(c4);
    allCourses.push_back(c5);
  }

  std::cout << "[INFO] Hafizadaki Toplam Ders Sayisi: " << allCourses.size() << "\n";

  std::cout << "\n>>> ADIM 2: Cakisim Grafi Insa Ediliyor...\n";
  Graph conflictGraph;
  Scheduler::buildConflictGraph(allCourses, conflictGraph);
  conflictGraph.printGraph();

  std::cout << "\n>>> ADIM 3: Graf Boyama Baslatiliyor...\n";
  HashMap<std::string, int> courseToSlotMap;
  int totalAvailableSlots = 5; 

  bool schedulingPossible = Scheduler::colorGraph(allCourses, conflictGraph, totalAvailableSlots, courseToSlotMap);

  if (!schedulingPossible) {
    std::cerr << "\n[TEST SONUCU] Zamanlama basarisiz! (Infeasibility Detected).\n";
    for (int i = 0; i < allCourses.size(); i++) delete allCourses.get(i);
    return 1;
  }

  std::cout << "\n>>> ADIM 4: Sinif Atama Modulu Baslatiliyor...\n";
  DynamicArray<Room> allRooms;
  allRooms.push_back(Room("D101", 2));   
  allRooms.push_back(Room("D102", 3));   
  allRooms.push_back(Room("Amfi-A", 5)); 

  // FRONTEND İÇİN HARİTA OLUŞTURULDU
  HashMap<std::string, std::string> courseRooms;

  // HARİTA SCHEDULER'A VERİLDİ
  Scheduler::assignRooms(allCourses, allRooms, courseToSlotMap, totalAvailableSlots, courseRooms);

  std::cout << "\n>>> ADIM 5: Web Arayuzu Uretiliyor...\n";
  // FRONTEND ÇAĞRILDI
  Frontend::generateWebInterface(allCourses, courseToSlotMap, courseRooms, totalAvailableSlots);

  std::cout << "\n=========================================================================\n";
  std::cout << "--- Tum Modullerin Testi Basariyla Tamamlandi ------\n";
  std::cout << "=========================================================================\n";

  for (int i = 0; i < allCourses.size(); i++) {
    delete allCourses.get(i);
  }

  return 0;
}