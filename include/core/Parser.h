#ifndef PARSER_H
#define PARSER_H

#include "../data_structures/DynamicArray.h" // Dinamik diziyi dahil ettik
#include "../data_structures/LinkedList.h"
#include "Course.h"
#include "Room.h"
#include <fstream>
#include <iostream>
#include <string>

class Parser {
public:
  /**
   * @brief JSON dosyasini okur ve parcaladigi dersleri DynamicArray'e doldurur.
   * @param filename Okunacak dosyanin yolu
   * @param allCourses Derslerin kaydedilecegi dinamik dizi (Referans olarak
   * alinir)
   */

  // PARAMETREDEKI Course KISMINI Course* YAPTIK
  static void parseJSON(const std::string &filename,
                        DynamicArray<Course *> &allCourses) {
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
      std::cerr << "[HATA] Dosya acilamadi: " << filename << std::endl;
      return;
    }

    std::cout << "[PARSER] Dosya okunuyor: " << filename << "...\n";

    std::string content = "";
    std::string tempLine;
    while (std::getline(file, tempLine)) {
      content += tempLine + " ";
    }
    file.close();

    size_t currentPos = 0;
    while ((currentPos = content.find("{", currentPos)) != std::string::npos) {
      size_t endPos = content.find("}", currentPos);
      if (endPos == std::string::npos)
        break;

      std::string block = content.substr(currentPos, endPos - currentPos);

      if (block.find("\"id\"") != std::string::npos &&
          block.find("\"name\"") != std::string::npos) {

        std::string courseId = extractValue(block, "\"id\"");
        std::string courseName = extractValue(block, "\"name\"");
        LinkedList<std::string> students = extractArray(block, "\"enrolled\"");

        // BURASI DEĞİŞTİ: Nesneyi "new" ile pointer olarak olusturuyoruz
        Course *newCourse = new Course(courseId, courseName);

        Node<std::string> *current = students.getHead();
        while (current != NULL) {
          newCourse->addStudent(
              current->data); // Pointer oldugu icin '.' yerine '->' kullandik
          current = current->next;
        }

        allCourses.push_back(newCourse);
      }
      currentPos = endPos + 1;
    }

    std::cout << "[PARSER] Toplam " << allCourses.size()
              << " ders basariyla hafizaya alindi.\n";
  }

private:
  static std::string extractValue(const std::string &line,
                                  const std::string &key) {
    size_t keyPos = line.find(key);
    if (keyPos == std::string::npos)
      return "";
    size_t colonPos = line.find(":", keyPos);
    if (colonPos == std::string::npos)
      return "";
    size_t firstQuote = line.find("\"", colonPos);
    if (firstQuote == std::string::npos)
      return "";
    size_t secondQuote = line.find("\"", firstQuote + 1);
    if (secondQuote == std::string::npos)
      return "";
    return line.substr(firstQuote + 1, secondQuote - firstQuote - 1);
  }

  static LinkedList<std::string> extractArray(const std::string &line,
                                              const std::string &key) {
    LinkedList<std::string> list;
    size_t keyPos = line.find(key);
    if (keyPos == std::string::npos)
      return list;
    size_t bracketStart = line.find("[", keyPos);
    if (bracketStart == std::string::npos)
      return list;
    size_t bracketEnd = line.find("]", bracketStart);
    if (bracketEnd == std::string::npos)
      return list;

    std::string arrayContent =
        line.substr(bracketStart + 1, bracketEnd - bracketStart - 1);
    size_t startQuote = arrayContent.find("\"");
    while (startQuote != std::string::npos) {
      size_t endQuote = arrayContent.find("\"", startQuote + 1);
      if (endQuote != std::string::npos) {
        std::string value =
            arrayContent.substr(startQuote + 1, endQuote - startQuote - 1);
        list.insert(value);
        startQuote = arrayContent.find("\"", endQuote + 1);
      } else {
        break;
      }
    }
    return list;
  }
};

#endif // PARSER_H