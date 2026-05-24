#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "../data_structures/DynamicArray.h"
#include "../data_structures/Graph.h"
#include "Course.h"
#include "Room.h"
#include "../data_structures/HashMap.h"
#include <iostream>

/**
 * @brief Class responsible for calculating exam conflicts and scheduling times/rooms.
 */
class Scheduler {
public:
  /**
   * @brief Compares enrolled students across courses to construct the Conflict Graph.
   * @param allCourses List of all courses loaded from JSON
   * @param conflictGraph Empty graph object where conflict edges will be added
   */
  static void buildConflictGraph(DynamicArray<Course *> &allCourses,
                                 Graph &conflictGraph) {
    // 1. Add all courses to the graph as vertices
    for (int i = 0; i < allCourses.size(); i++) {
      conflictGraph.addVertex(allCourses.get(i)->id);
    }

    // 2. Intersect courses to detect common students (create edges)
    for (int i = 0; i < allCourses.size(); i++) {
      for (int j = i + 1; j < allCourses.size(); j++) {
        bool conflictFound = false;
        Node<std::string> *studentA = allCourses.get(i)->enrolledStudents.getHead();

        while (studentA != NULL && !conflictFound) {
          Node<std::string> *studentB = allCourses.get(j)->enrolledStudents.getHead();
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

  /**
   * @brief Colors the conflict graph using the Largest-Degree-First (Welsh-Powell) heuristic.
   * @return False if a valid timetable cannot be scheduled within totalAvailableSlots.
   */
  static bool colorGraph(DynamicArray<Course *> &allCourses,
                         Graph &conflictGraph,
                         int totalAvailableSlots,
                         HashMap<std::string, int> &courseToSlotMap) {
    int n = allCourses.size();
    if (n == 0) return true;

    // Create a temporary array for sorting course pointers
    DynamicArray<Course *> sortedCourses;
    for (int i = 0; i < n; i++) {
      sortedCourses.push_back(allCourses.get(i));
    }

    // 1. Welsh-Powell Strategy: Sort courses by their degree in descending order
    for (int i = 0; i < n - 1; i++) {
      for (int j = 0; j < n - i - 1; j++) {
        int degA = conflictGraph.getDegree(sortedCourses.get(j)->id);
        int degB = conflictGraph.getDegree(sortedCourses.get(j + 1)->id);
        if (degA < degB) {
          Course *temp = sortedCourses.get(j);
          sortedCourses.set(j, sortedCourses.get(j + 1));
          sortedCourses.set(j + 1, temp);
        }
      }
    }

    // Initialize all courses in the map with an unassigned color index (-1)
    for (int i = 0; i < n; i++) {
      courseToSlotMap.insert(sortedCourses.get(i)->id, -1);
    }

    // 2. Greedy Coloring Phase
    for (int i = 0; i < n; i++) {
      Course *currentCourse = sortedCourses.get(i);

      bool *availableColors = new bool[n];
      for (int k = 0; k < n; k++) availableColors[k] = true;

      // Check neighbor assignments
      const LinkedList<std::string>& neighbors = conflictGraph.getNeighbors(currentCourse->id);
      Node<std::string> *neighborNode = neighbors.getHead();

      while (neighborNode != NULL) {
        std::string neighborId = neighborNode->data;
        int *neighborColor = courseToSlotMap.get(neighborId);

        if (neighborColor != NULL && *neighborColor != -1) {
          availableColors[*neighborColor] = false;
        }
        neighborNode = neighborNode->next;
      }

      int chosenColor = -1;
      for (int c = 0; c < n; c++) {
        if (availableColors[c]) {
          chosenColor = c;
          break;
        }
      }
      delete[] availableColors; // Prevent memory leak
      
      // INFEASIBILITY DETECTION
      if (chosenColor >= totalAvailableSlots) {
        std::cerr << "[ERROR] Infeasibility Detected: Not enough Time Slots to schedule exams without conflicts!\n";
        return false;
      }

      // Save the valid time slot assignment
      courseToSlotMap.insert(currentCourse->id, chosenColor);
    }
    return true;
  }

  /**
   * @brief Packs courses into classrooms using the First-Fit Decreasing approach.
   */
  static void assignRooms(DynamicArray<Course *> &allCourses,
                          DynamicArray<Room> &allRooms,
                          HashMap<std::string, int> &courseToSlotMap,
                          int totalSlots) {
    
    std::cout << "\n=============================================\n";
    std::cout << "--- Room Assignment Module (Bin Packing) ----\n";
    std::cout << "=============================================\n";

    // Handle each time slot independently as a separate bin-packing problem
    for (int slot = 0; slot < totalSlots; slot++) {
        
      // 1. Filter courses scheduled for this specific time slot
      DynamicArray<Course *> coursesInThisSlot;
      for (int i = 0; i < allCourses.size(); i++) {
        int *assignedSlot = courseToSlotMap.get(allCourses.get(i)->id);
        if (assignedSlot != NULL && *assignedSlot == slot) {
          coursesInThisSlot.push_back(allCourses.get(i));
        }
      }

      if (coursesInThisSlot.size() == 0) continue;

      // 2. FFD Strategy: Sort courses by enrollment size in descending order
      for (int i = 0; i < coursesInThisSlot.size() - 1; i++) {
        for (int j = 0; j < coursesInThisSlot.size() - i - 1; j++) {
          int sizeA = coursesInThisSlot.get(j)->enrolledStudents.getSize();
          int sizeB = coursesInThisSlot.get(j + 1)->enrolledStudents.getSize();
          if (sizeA < sizeB) {
            Course *temp = coursesInThisSlot.get(j);
            coursesInThisSlot.set(j, coursesInThisSlot.get(j + 1));
            coursesInThisSlot.set(j + 1, temp);
          }
        }
      }

      DynamicArray<Room> localRooms;
      for (int r = 0; r < allRooms.size(); r++) {
        localRooms.push_back(allRooms.get(r));
      }

      // Sort rooms by total capacity in descending order to optimize First-Fit matching
      for (int i = 0; i < localRooms.size() - 1; i++) {
        for (int j = 0; j < localRooms.size() - i - 1; j++) {
          if (localRooms.get(j).capacity < localRooms.get(j + 1).capacity) {
            Room temp = localRooms.get(j);
            localRooms.set(j, localRooms.get(j + 1));
            localRooms.set(j + 1, temp);
          }
        }
      }

      // 3. Classroom Assignment Phase
      std::cout << "\n[Time Slot " << slot + 1 << " / (Color " << slot << ")] Exams:\n";
      for (int i = 0; i < coursesInThisSlot.size(); i++) {
        Course *course = coursesInThisSlot.get(i);
        int studentsRemaining = course->enrolledStudents.getSize();

        std::cout << "  -> Course: " << course->id << " (" << studentsRemaining << " Students) -> Assigned Room(s): ";

        for (int r = 0; r < localRooms.size() && studentsRemaining > 0; r++) {
          // 1. Get a copy of the current room state
          Room room = localRooms.get(r); 

          if (room.capacity > 0) {
            if (room.capacity >= studentsRemaining) {
              // Single room is sufficient
              std::cout << room.id << " [" << studentsRemaining << " seats reserved] ";
              room.capacity -= studentsRemaining;
              studentsRemaining = 0;
            } else {
              // Multiple room combining rule
              std::cout << room.id << " [" << room.capacity << " seats merged] + ";
              studentsRemaining -= room.capacity;
              room.capacity = 0; 
            }
            
            // 2. Save the updated room state back into the DynamicArray
            localRooms.set(r, room); 
          }
        }

        if (studentsRemaining > 0) {
          std::cout << " -> !!! [CAPACITY INSUFFICIENT: " << studentsRemaining << " students left unallocated!]";
        }
        std::cout << "\n";
      }
    }
  }
};

#endif // SCHEDULER_H