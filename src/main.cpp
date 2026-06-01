/**
 * @file main.cpp
 * @brief PROJ-16 Exam Timetabling System main file.
 *
 * This file sequentially executes the steps of reading data from JSON,
 * constructing the conflict graph, graph coloring (time slot assignment),
 * bin packing (room assignment), and generating the web interface.
 *
 * @authors Mert Deha Karayel - Serkan Can Erdem - Selin Yaslan
 * @date May 2026
 */

#include <iostream>
#include <string>

#include "../include/core/Course.h"
#include "../include/core/Frontend.h"
#include "../include/core/Parser.h"
#include "../include/core/Scheduler.h"
#include "../include/data_structures/DynamicArray.h"
#include "../include/data_structures/Graph.h"
#include "../include/data_structures/HashMap.h"
#include "../include/data_structures/LinkedList.h"

int main(int argc, char *argv[]) {
    std::cout << "=========================================================================\n";
    std::cout << "--- PROJ-16: Exam Timetabling System ---\n";
    std::cout << "=========================================================================\n\n";

    DynamicArray<Course *> allCourses;
    DynamicArray<Room> allRooms;

    // Default test file
    std::string inputFile = "data/input_sample.json";
    if (argc > 1) {
        inputFile = argv[1];  // Retrieve the file entered from the terminal
    }

    std::cout << "\n>>> STEP 1: Reading Data From JSON File...\n";
    Parser::parseJSON(inputFile, allCourses);
    Parser::parseRooms(inputFile, allRooms);

    std::cout << "[INFO] Total Number of Courses in Memory: " << allCourses.size() << "\n";
    std::cout << "[INFO] Total Number of Rooms in Memory: " << allRooms.size() << "\n";

    // If data is completely empty, safely shut down the system.
    if (allCourses.size() == 0) {
        std::cout << "\n[WARNING] No courses found in the parsed dataset! Program terminating safely.\n";

        // To prevent the old HTML file from displaying misleading information, we overwrite it with an empty warning
        // message:
        std::ofstream htmlFile("frontend/exam_schedule.html");
        if (htmlFile.is_open()) {
            htmlFile << "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>No Data</title></head>";
            htmlFile
                << "<body style='font-family:sans-serif; text-align:center; padding:50px; background-color:#f8f9fa;'>";
            htmlFile << "<h2 style='color:#e74c3c;'>No courses found in the input file (Or the file could not be "
                        "opened).</h2>";
            htmlFile << "<p style='color:#7f8c8d;'>There is no exam schedule to display.</p>";
            htmlFile << "</body></html>";
            htmlFile.close();
            std::cout << "[FRONTEND] Old HTML file cleared (Empty screen generated).\n";
        }

        return 0;
    }

    std::cout << "\n>>> STEP 2: Constructing Conflict Graph...\n";
    Graph conflictGraph;
    Scheduler::buildConflictGraph(allCourses, conflictGraph);
    conflictGraph.printGraph();

    std::cout << "\n>>> STEP 3: Starting Graph Coloring...\n";
    HashMap<std::string, int> courseToSlotMap;
    int totalAvailableSlots = allCourses.size();

    bool schedulingPossible = Scheduler::colorGraph(allCourses, conflictGraph, totalAvailableSlots, courseToSlotMap);

    if (!schedulingPossible) {
        std::cerr << "\n[TEST RESULT] Scheduling failed! (Infeasibility Detected).\n";
        for (int i = 0; i < allCourses.size(); i++) delete allCourses.get(i);
        return 1;
    }

    std::cout << "\n>>> STEP 4: Starting Room Assignment Module...\n";
    HashMap<std::string, std::string> courseRooms;
    Scheduler::assignRooms(allCourses, allRooms, courseToSlotMap, totalAvailableSlots, courseRooms);

    std::cout << "\n>>> STEP 5: Generating Web Interface...\n";
    Frontend::generateWebInterface(allCourses, courseToSlotMap, courseRooms, totalAvailableSlots);

    std::cout << "\n=========================================================================\n";
    std::cout << "--- All Module Tests Completed Successfully ------\n";
    std::cout << "=========================================================================\n";

    for (int i = 0; i < allCourses.size(); i++) {
        delete allCourses.get(i);
    }

    return 0;
}