#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <iostream>
#include <string>

#include "../data_structures/DynamicArray.h"
#include "../data_structures/Graph.h"
#include "../data_structures/HashMap.h"
#include "Course.h"
#include "Room.h"

/**
 * @file Scheduler.h
 * @brief Contains the exam scheduling (Graph Coloring) and room assignment (Bin Packing) module.
 */

/**
 * @brief Class that builds the conflict graph, colors the graph using Welsh-Powell, and assigns rooms via First-Fit
 * Decreasing.
 */
class Scheduler {
   public:
    /**
     * @brief Detects shared students between courses to construct the Conflict Graph.
     * @param allCourses Dynamic array containing all courses in the system.
     * @param conflictGraph Graph object onto which conflict edges will be added.
     * @post An edge is created on the graph between any courses that share at least one student.
     */
    static void buildConflictGraph(DynamicArray<Course *> &allCourses, Graph &conflictGraph) {
        for (int i = 0; i < allCourses.size(); i++) {
            conflictGraph.addVertex(allCourses.get(i)->id);
        }
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
     * @brief Assigns a time slot (color) to the graph using the Welsh-Powell (Largest-Degree-First) algorithm.
     * @param allCourses Dynamic array containing all courses in the system.
     * @param conflictGraph Graph object representing the conflicts.
     * @param totalAvailableSlots Maximum number of usable time slots.
     * @param courseToSlotMap Hash Map where assigned time slots will be recorded.
     * @return bool Returns true if assignment is successful, false in case of capacity insufficiency (Infeasibility).
     * @post A time slot value is added for each course inside courseToSlotMap.
     */
    static bool colorGraph(DynamicArray<Course *> &allCourses, Graph &conflictGraph, int totalAvailableSlots,
                           HashMap<std::string, int> &courseToSlotMap) {
        int n = allCourses.size();
        if (n == 0) return true;

        DynamicArray<Course *> sortedCourses;
        for (int i = 0; i < n; i++) sortedCourses.push_back(allCourses.get(i));

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

        for (int i = 0; i < n; i++) courseToSlotMap.insert(sortedCourses.get(i)->id, -1);

        for (int i = 0; i < n; i++) {
            Course *currentCourse = sortedCourses.get(i);
            bool *availableColors = new bool[n];
            for (int k = 0; k < n; k++) availableColors[k] = true;

            const LinkedList<std::string> &neighbors = conflictGraph.getNeighbors(currentCourse->id);
            Node<std::string> *neighborNode = neighbors.getHead();

            while (neighborNode != NULL) {
                int *neighborColor = courseToSlotMap.get(neighborNode->data);
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
            delete[] availableColors;

            if (chosenColor >= totalAvailableSlots) {
                std::cerr << "[ERROR] Infeasibility Detected!\n";
                return false;
            }
            courseToSlotMap.insert(currentCourse->id, chosenColor);
        }
        return true;
    }

    /**
     * @brief Assigns exams to rooms using the First-Fit Decreasing (FFD) algorithm (Bin Packing).
     * @param allCourses Dynamic array containing all courses in the system.
     * @param allRooms Dynamic array containing available classrooms in the school.
     * @param courseToSlotMap Time slot assignments calculated by colorGraph.
     * @param totalSlots Total number of time slots.
     * @param courseRooms Hash Map that will hold the course-room string outputs generated for the frontend.
     * @post A room assignment text (std::string) is added to the courseRooms map for each course.
     */
    // UPDATED HERE: courseRooms added to transfer data to Frontend
    static void assignRooms(DynamicArray<Course *> &allCourses, DynamicArray<Room> &allRooms,
                            HashMap<std::string, int> &courseToSlotMap, int totalSlots,
                            HashMap<std::string, std::string> &courseRooms) {
        std::cout << "\n=============================================\n";
        std::cout << "--- Room Assignment Module (Bin Packing) ----\n";
        std::cout << "=============================================\n";

        for (int slot = 0; slot < totalSlots; slot++) {
            DynamicArray<Course *> coursesInThisSlot;
            for (int i = 0; i < allCourses.size(); i++) {
                int *assignedSlot = courseToSlotMap.get(allCourses.get(i)->id);
                if (assignedSlot != NULL && *assignedSlot == slot) {
                    coursesInThisSlot.push_back(allCourses.get(i));
                }
            }

            if (coursesInThisSlot.size() == 0) continue;

            for (int i = 0; i < coursesInThisSlot.size() - 1; i++) {
                for (int j = 0; j < coursesInThisSlot.size() - i - 1; j++) {
                    if (coursesInThisSlot.get(j)->enrolledStudents.getSize() <
                        coursesInThisSlot.get(j + 1)->enrolledStudents.getSize()) {
                        Course *temp = coursesInThisSlot.get(j);
                        coursesInThisSlot.set(j, coursesInThisSlot.get(j + 1));
                        coursesInThisSlot.set(j + 1, temp);
                    }
                }
            }

            DynamicArray<Room> localRooms;
            for (int r = 0; r < allRooms.size(); r++) localRooms.push_back(allRooms.get(r));

            for (int i = 0; i < localRooms.size() - 1; i++) {
                for (int j = 0; j < localRooms.size() - i - 1; j++) {
                    if (localRooms.get(j).capacity < localRooms.get(j + 1).capacity) {
                        Room temp = localRooms.get(j);
                        localRooms.set(j, localRooms.get(j + 1));
                        localRooms.set(j + 1, temp);
                    }
                }
            }

            std::cout << "\n[Time Slot " << slot + 1 << " / (Color " << slot << ")] Exams:\n";
            for (int i = 0; i < coursesInThisSlot.size(); i++) {
                Course *course = coursesInThisSlot.get(i);
                int studentsRemaining = course->enrolledStudents.getSize();

                std::string assignedRoomStr = "";  // TEXT TO BE SENT TO HTML

                std::cout << "  -> Course: " << course->id << " (" << studentsRemaining
                          << " Students) -> Assigned Room(s): ";

                for (int r = 0; r < localRooms.size() && studentsRemaining > 0; r++) {
                    Room room = localRooms.get(r);

                    if (room.capacity > 0) {
                        if (room.capacity >= studentsRemaining) {
                            std::cout << room.id << " [" << studentsRemaining << " seats reserved] ";
                            assignedRoomStr += room.id;  // SAVE ROOM NAME
                            room.capacity -= studentsRemaining;
                            studentsRemaining = 0;
                        } else {
                            std::cout << room.id << " [" << room.capacity << " seats merged] + ";
                            assignedRoomStr += room.id + " + ";  // SAVE ROOM NAME
                            studentsRemaining -= room.capacity;
                            room.capacity = 0;
                        }
                        localRooms.set(r, room);
                    }
                }

                if (studentsRemaining > 0) {
                    std::cout << " -> !!! [CAPACITY INSUFFICIENT]";
                    assignedRoomStr = "Insufficient Capacity / Unassigned";
                    assignedRoomStr += " (No space left!)";
                }
                std::cout << "\n";

                // ADD RESULT TO MAP (Frontend will read from here)
                courseRooms.insert(course->id, assignedRoomStr);
            }
        }
    }
};

#endif  // SCHEDULER_H