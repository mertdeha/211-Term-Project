#ifndef PARSER_H
#define PARSER_H

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "../data_structures/DynamicArray.h"  // Included dynamic array
#include "../data_structures/LinkedList.h"
#include "Course.h"
#include "Room.h"

/**
 * @file Parser.h
 * @brief Class used to read and process data files in JSON format.
 */

/**
 * @brief Class that manually parses JSON format without using external libraries.
 */
class Parser {
   public:
    /**
     * @brief Reads a JSON file and populates the DynamicArray with parsed courses.
     *
     * @param filename File path of the JSON file to be read.
     * @param allCourses Reference to the dynamic array where read courses will be added.
     * @post Each course object read from the file is dynamically allocated in memory with 'new' and added to the array.
     */
    static void parseJSON(const std::string &filename, DynamicArray<Course *> &allCourses) {
        std::ifstream file(filename.c_str());
        if (!file.is_open()) {
            std::cerr << "[ERROR] File could not be opened: " << filename << std::endl;
            return;
        }

        std::cout << "[PARSER] Reading file: " << filename << "...\n";

        std::string content = "";
        std::string tempLine;
        while (std::getline(file, tempLine)) {
            content += tempLine + " ";
        }
        file.close();

        size_t currentPos = 0;
        while ((currentPos = content.find("{", currentPos)) != std::string::npos) {
            size_t endPos = content.find("}", currentPos);
            if (endPos == std::string::npos) break;

            std::string block = content.substr(currentPos, endPos - currentPos);

            if (block.find("\"id\"") != std::string::npos && block.find("\"name\"") != std::string::npos) {
                std::string courseId = extractValue(block, "\"id\"");
                std::string courseName = extractValue(block, "\"name\"");
                LinkedList<std::string> students = extractArray(block, "\"enrolled\"");

                Course *newCourse = new Course(courseId, courseName);

                Node<std::string> *current = students.getHead();
                while (current != NULL) {
                    newCourse->addStudent(current->data);
                    current = current->next;
                }

                allCourses.push_back(newCourse);
            }
            currentPos = endPos + 1;
        }

        std::cout << "[PARSER] Total of " << allCourses.size() << " courses successfully loaded into memory.\n";
    }

    /**
     * @brief Reads classroom (room) data from a JSON file and adds them to the array.
     *
     * @param filename File path of the file to be read.
     * @param allRooms Reference to the dynamic array where read rooms will be added.
     * @post Data inside the "rooms" block found in the file is processed and saved into the array.
     */
    static void parseRooms(const std::string &filename, DynamicArray<Room> &allRooms) {
        std::ifstream file(filename.c_str());
        if (!file.is_open()) return;

        std::string content = "";
        std::string tempLine;
        while (std::getline(file, tempLine)) {
            content += tempLine + " ";
        }
        file.close();

        size_t roomsSection = content.find("\"rooms\"");
        if (roomsSection == std::string::npos) return;

        size_t currentPos = roomsSection;
        while ((currentPos = content.find("{", currentPos)) != std::string::npos) {
            size_t endPos = content.find("}", currentPos);
            if (endPos == std::string::npos) break;

            std::string block = content.substr(currentPos, endPos - currentPos);

            if (block.find("\"capacity\"") != std::string::npos) {
                std::string roomId = extractValue(block, "\"id\"");
                std::string capStr = extractValue(block, "\"capacity\"");

                if (capStr.empty()) {
                    size_t capPos = block.find("\"capacity\"");
                    size_t colonPos = block.find(":", capPos);
                    size_t commaPos = block.find_first_of(",}", colonPos);
                    capStr = block.substr(colonPos + 1, commaPos - colonPos - 1);
                }

                int capacity = std::atoi(capStr.c_str());
                allRooms.push_back(Room(roomId, capacity));
            }
            currentPos = endPos + 1;
        }
    }

   private:
    /**
     * @brief Extracts the value of a specified key from a JSON block.
     * @param line The text block to be searched.
     * @param key The key of the value to be extracted (e.g., "id").
     * @return std::string The string value corresponding to the key.
     */
    static std::string extractValue(const std::string &line, const std::string &key) {
        size_t keyPos = line.find(key);
        if (keyPos == std::string::npos) return "";
        size_t colonPos = line.find(":", keyPos);
        if (colonPos == std::string::npos) return "";
        size_t firstQuote = line.find("\"", colonPos);
        if (firstQuote == std::string::npos) return "";
        size_t secondQuote = line.find("\"", firstQuote + 1);
        if (secondQuote == std::string::npos) return "";
        return line.substr(firstQuote + 1, secondQuote - firstQuote - 1);
    }

    /**
     * @brief Reads list (array) data inside JSON and assigns it to a linked list.
     * @param line The text block to be searched.
     * @param key The key of the list data (e.g., "enrolled").
     * @return LinkedList<std::string> A linked list containing the read list data.
     */
    static LinkedList<std::string> extractArray(const std::string &line, const std::string &key) {
        LinkedList<std::string> list;
        size_t keyPos = line.find(key);
        if (keyPos == std::string::npos) return list;
        size_t bracketStart = line.find("[", keyPos);
        if (bracketStart == std::string::npos) return list;
        size_t bracketEnd = line.find("]", bracketStart);
        if (bracketEnd == std::string::npos) return list;

        std::string arrayContent = line.substr(bracketStart + 1, bracketEnd - bracketStart - 1);
        size_t startQuote = arrayContent.find("\"");
        while (startQuote != std::string::npos) {
            size_t endQuote = arrayContent.find("\"", startQuote + 1);
            if (endQuote != std::string::npos) {
                std::string value = arrayContent.substr(startQuote + 1, endQuote - startQuote - 1);
                list.insert(value);
                startQuote = arrayContent.find("\"", endQuote + 1);
            } else {
                break;
            }
        }
        return list;
    }
};

#endif  // PARSER_H