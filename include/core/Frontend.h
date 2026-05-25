#ifndef FRONTEND_H
#define FRONTEND_H

#include "../data_structures/DynamicArray.h"
#include "../data_structures/HashMap.h"
#include "Course.h"
#include <iostream>
#include <fstream>
#include <string>

class Frontend {
public:
  // DİKKAT: Artık 4 parametre alıyor (courseRooms eklendi)
  static void generateWebInterface(DynamicArray<Course *> &allCourses, 
                                   HashMap<std::string, int> &timeSlots, 
                                   HashMap<std::string, std::string> &courseRooms, 
                                   int totalSlots) {
    std::ofstream htmlFile("sinav_takvimi.html");
    if (!htmlFile.is_open()) return;

    htmlFile << "<!DOCTYPE html>\n<html lang='tr'>\n<head>\n<meta charset='UTF-8'>\n";
    htmlFile << "<title>PROJ-16 Sinav Takvimi</title>\n";
    htmlFile << "<style>\n";
    htmlFile << "body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; background-color: #f8f9fa; margin: 40px; color: #333; }\n";
    htmlFile << "h1 { text-align: center; color: #2c3e50; }\n";
    htmlFile << ".container { max-width: 900px; margin: 0 auto; background: white; padding: 20px; border-radius: 10px; box-shadow: 0 4px 8px rgba(0,0,0,0.05); }\n";
    htmlFile << "table { width: 100%; border-collapse: collapse; margin-top: 20px; }\n";
    htmlFile << "th, td { padding: 15px; border-bottom: 1px solid #ddd; text-align: left; }\n";
    htmlFile << "th { background-color: #3498db; color: white; font-weight: bold; }\n";
    htmlFile << ".slot-row { background-color: #2c3e50; color: white; font-weight: bold; text-align: center; }\n";
    htmlFile << "tr:hover { background-color: #f1f1f1; }\n";
    htmlFile << "</style>\n</head>\n<body>\n";
    
    htmlFile << "<div class='container'>\n";
    htmlFile << "<h1>📅 Otomatik Sınav Çizelgesi</h1>\n<table>\n";
    htmlFile << "<tr><th>Ders Kodu</th><th>Ders Adı</th><th>Atanan Sınıf(lar)</th></tr>\n";

    for (int currentSlot = 0; currentSlot < totalSlots; currentSlot++) {
      htmlFile << "<tr class='slot-row'><td colspan='3'>ZAMAN DİLİMİ " << currentSlot + 1 << "</td></tr>\n";
      
      bool hasExams = false;
      for (int i = 0; i < allCourses.size(); i++) {
        std::string courseId = allCourses.get(i)->id;
        int *assignedSlot = timeSlots.get(courseId);

        if (assignedSlot != NULL && *assignedSlot == currentSlot) {
            std::string *roomPtr = courseRooms.get(courseId);
            std::string assignedRoom = (roomPtr != NULL) ? *roomPtr : "Atanamadı";

            htmlFile << "<tr><td>" << courseId << "</td><td>" << allCourses.get(i)->name << "</td><td>" << assignedRoom << "</td></tr>\n";
            hasExams = true;
        }
      }
      if (!hasExams) htmlFile << "<tr><td colspan='3' style='text-align:center; color:#7f8c8d;'>Bu dilimde sınav yok.</td></tr>\n";
    }

    htmlFile << "</table>\n</div>\n</body>\n</html>\n";
    htmlFile.close();
    
    std::cout << "\n[FRONTEND] Web arayuzu 'sinav_takvimi.html' adli dosyaya basariyla kaydedildi!\n" << std::endl;
  }
};
#endif