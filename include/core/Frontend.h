#ifndef FRONTEND_H
#define FRONTEND_H

#include "../data_structures/DynamicArray.h"
#include "../data_structures/HashMap.h"
#include "Course.h"
#include <fstream>
#include <iostream>
#include <string>

class Frontend {
public:
  static void generateWebInterface(
      DynamicArray<Course *> &allCourses, HashMap<std::string, int> &timeSlots,
      HashMap<std::string, std::string> &courseRooms, int totalSlots) {
    std::ofstream htmlFile("frontend/exam_schedule.html");
    if (!htmlFile.is_open())
      return;

    htmlFile << "<!DOCTYPE html>\n<html lang='en'>\n<head>\n<meta "
                "charset='UTF-8'>\n";
    htmlFile << "<title>PROJ-16 Exam Schedule</title>\n";
    htmlFile << "<style>\n";
    htmlFile << "body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, "
                "sans-serif; background-color: #f8f9fa; margin: 40px; color: "
                "#333; }\n";
    htmlFile << "h1 { text-align: center; color: #2c3e50; }\n";
    htmlFile << ".container { max-width: 900px; margin: 0 auto; background: "
                "white; padding: 20px; border-radius: 10px; box-shadow: 0 4px "
                "8px rgba(0,0,0,0.05); }\n";
    htmlFile << "table { width: 100%; border-collapse: collapse; margin-top: "
                "20px; }\n";
    htmlFile << "th, td { padding: 15px; border-bottom: 1px solid #ddd; "
                "text-align: left; transition: all 0.2s ease-in-out; }\n";
    htmlFile << "th { background-color: #3498db; color: white; font-weight: "
                "bold; }\n";

    // Time slot satırının normal hali ve yumuşak geçiş animasyonu (transition)
    // eklendi
    htmlFile
        << ".slot-row { background-color: #2c3e50; color: white; font-weight: "
           "bold; text-align: center; letter-spacing: 1px; }\n";

    htmlFile << "tr:not(.slot-row):hover { background-color: #f1f1f1; }\n";

    htmlFile << ".slot-row:hover { background-color: #1abc9c; color: white; "
                "transform: scale(1.01); }\n";

    htmlFile << "</style>\n</head>\n<body>\n";

    htmlFile << "<div class='container'>\n";
    htmlFile << "<h1>📅 Automated Exam Scheduler</h1>\n<table>\n";
    htmlFile << "<tr><th>Course Code</th><th>Course Name</th><th>Assigned "
                "Room(s)</th></tr>\n";

    for (int currentSlot = 0; currentSlot < totalSlots; currentSlot++) {
      htmlFile << "<tr class='slot-row'><td colspan='3'>TIME SLOT "
               << currentSlot + 1 << "</td></tr>\n";

      bool hasExams = false;
      for (int i = 0; i < allCourses.size(); i++) {
        std::string courseId = allCourses.get(i)->id;
        int *assignedSlot = timeSlots.get(courseId);

        if (assignedSlot != NULL && *assignedSlot == currentSlot) {
          std::string *roomPtr = courseRooms.get(courseId);
          std::string assignedRoom =
              (roomPtr != NULL) ? *roomPtr : "Not Assigned";

          htmlFile << "<tr><td>" << courseId << "</td><td>"
                   << allCourses.get(i)->name << "</td><td>" << assignedRoom
                   << "</td></tr>\n";
          hasExams = true;
        }
      }
      if (!hasExams)
        htmlFile << "<tr><td colspan='3' style='text-align:center; "
                    "color:#7f8c8d;'>No exams in this time slot.</td></tr>\n";
    }

    htmlFile << "</table>\n</div>\n</body>\n</html>\n";
    htmlFile.close();

    std::cout << "\n[FRONTEND] Web interface successfully generated as "
                 "'exam_schedule.html'!\n"
              << std::endl;
  }
};
#endif