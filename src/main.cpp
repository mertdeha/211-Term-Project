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
  std::cout << "=========================================================================\n";
  std::cout << "--- PROJ-16: Sinav Cizelgeleme Sistemi---\n";
  std::cout << "=========================================================================\n\n";

  // 1. VERILERI HAZIRLA
  DynamicArray<Course *> allCourses;

  // DOSYA OKUMA VE SIMÜLASYON GÜVENLİK ÖNLEMİ:
  // Eğer JSON dosyası bulunamazsa testin yarıda kalmaması için otomatik örnek veriler yüklenecektir.
  std::string inputFile = "data/input_sample.json";
  if (argc > 1) {
    inputFile = argv[1];
  }

  // Parser'ı çalıştırmayı dene
  Parser::parseJSON(inputFile, allCourses);

  // Eğer dosya bulunamadıysa ve dizi boş kaldıysa simülasyon verisini kod içinden besle
  if (allCourses.size() == 0) {
    std::cout << "[INFO] '" << inputFile << "' bulunamadi veya bos. Test verileri otomatik uretiliyor...\n";
    
    // Test Dersleri Oluşturuluyor
    Course *c1 = new Course("CSE101", "Intro to CS");
    c1->addStudent("Ahmet"); c1->addStudent("Mehmet"); c1->addStudent("Can");
    
    Course *c2 = new Course("CSE102", "Data Structures"); // CSE101 ile çakışıyor (Mehmet ortak)
    c2->addStudent("Mehmet"); c2->addStudent("Elif"); c2->addStudent("Zeynep");
    
    Course *c3 = new Course("MATH101", "Calculus I"); // CSE101 ile çakışıyor (Ahmet ortak)
    c3->addStudent("Ahmet"); c3->addStudent("Buse"); c3->addStudent("Kaan");
    
    Course *c4 = new Course("PHYS101", "Physics I"); // CSE102 ile çakışıyor (Elif ortak)
    c4->addStudent("Elif"); c4->addStudent("Kaan"); c4->addStudent("Arda");

    Course *c5 = new Course("CSE211", "Digital Design"); // Kalabalık ders (Çoklu oda birleştirmeyi tetikler)
    c5->addStudent("Seda"); c5->addStudent("Umut"); c5->addStudent("Omer"); 
    c5->addStudent("Ali"); c5->addStudent("Veli"); c5->addStudent("Ayse");

    allCourses.push_back(c1);
    allCourses.push_back(c2);
    allCourses.push_back(c3);
    allCourses.push_back(c4);
    allCourses.push_back(c5);
  }

  std::cout << "[INFO] Hafizadaki Toplam Ders Sayisi: " << allCourses.size() << "\n";

  // 2. CAKISMA GRAFINI KUR
  std::cout << "\n>>> ADIM 2: Cakisim Grafi Insa Ediliyor...\n";
  Graph conflictGraph;
  Scheduler::buildConflictGraph(allCourses, conflictGraph);

  std::cout << "\n[BASARILI] Graf olusturuldu! Iste otomatik cakisim durumu:\n";
  conflictGraph.printGraph();

  // 3. GRAF BOYAMA (ZAMAN DİLİMİ ATAMASI)
  std::cout << "\n>>> ADIM 3: Graf Boyama (Zaman Dilimi Atamasi) Baslatiliyor...\n";
  
  HashMap<std::string, int> courseToSlotMap;
  int totalAvailableSlots = 5; // Maksimum kullanılabilir zaman dilimi sayısı

  // Welsh-Powell tabanlı boyama motorunu çağırıyoruz
  bool schedulingPossible = Scheduler::colorGraph(allCourses, conflictGraph, totalAvailableSlots, courseToSlotMap);

  if (!schedulingPossible) {
    std::cerr << "\n[TEST SONUCU] Zamanlama basarisiz! Elinizdeki " << totalAvailableSlots 
              << " slot bu cakisim grafini cozmek icin YETERSIZ (Infeasibility Detected).\n";
    
    for (int i = 0; i < allCourses.size(); i++) delete allCourses.get(i);
    return 1;
  }

  std::cout << "[TEST SONUCU] Graf Boyama Basarili! Tum derslere zaman dilimi (slot) atandi.\n";

  // 4. SINIF ATAMA MODÜLÜ (KUTU PAKETLEME / BIN PACKING)
  std::cout << "\n>>> ADIM 4: Sınıf Atama Modulu (Bin Packing) Baslatiliyor...\n";
  DynamicArray<Room> allRooms;

  // Çoklu oda birleştirme (Multiple Room Merging) kuralını test etmek için odaları ve küçük kapasiteleri ekliyoruz
  allRooms.push_back(Room("D101", 2));   // Çok küçük sınıf (Birleştirmeyi zorlamak için)
  allRooms.push_back(Room("D102", 3));   // Küçük sınıf
  allRooms.push_back(Room("Amfi-A", 5)); // Orta sınıf

  // First-Fit Decreasing ve çoklu oda mantığını çalıştırıyoruz
  Scheduler::assignRooms(allCourses, allRooms, courseToSlotMap, totalAvailableSlots);

  std::cout << "\n=========================================================================\n";
  std::cout << "--- Tum Modullerin Testi Basariyla Tamamlandi ------\n";
  std::cout << "=========================================================================\n";

  // 5. HAFIZA TEMİZLİĞİ
  for (int i = 0; i < allCourses.size(); i++) {
    delete allCourses.get(i);
  }

  return 0;
}