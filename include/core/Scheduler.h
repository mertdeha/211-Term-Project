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
 * @brief Sinav zamanlamasi (Graph Coloring) ve sinif atamasi (Bin Packing) modulunu iceren dosya.
 */

/**
 * @brief Cakisim grafini kuran, Welsh-Powell ile graf boyayan ve First-Fit Decreasing ile oda atayan sinif.
 */
class Scheduler {
   public:
    /**
     * @brief Dersler arasindaki ortak ogrencileri tespit ederek cakisim grafini (Conflict Graph) olusturur.
     * * @param allCourses Sistemdeki tum dersleri barindiran dinamik dizi.
     * @param conflictGraph Uzerine cakisim kenarlarinin (edges) eklenecegi graf nesnesi.
     * @post Ortak ogrencisi olan dersler arasinda graf uzerinde kenar (edge) olusturulur.
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
     * @brief Welsh-Powell (Largest-Degree-First) algoritmasi ile grafa zaman dilimi (renk) atar.
     * * @param allCourses Sistemdeki tum dersleri barindiran dinamik dizi.
     * @param conflictGraph Cakisimlari gosteren graf nesnesi.
     * @param totalAvailableSlots Maksimum kullanilabilecek zaman dilimi sayisi.
     * @param courseToSlotMap Atanan zaman dilimlerinin kaydedilecegi Hash Map.
     * @return bool Atama basariliysa true, kapasite yetersizliginde (Infeasibility) false doner.
     * @post courseToSlotMap icerisine her ders icin bir zaman dilimi degeri eklenmis olur.
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
     * @brief First-Fit Decreasing (FFD) algoritmasi ile sinavlari odalara atar (Bin Packing).
     * * @param allCourses Sistemdeki tum dersleri barindiran dinamik dizi.
     * @param allRooms Okuldaki musait siniflari barindiran dinamik dizi.
     * @param courseToSlotMap colorGraph ile hesaplanan zaman dilimi atamalari.
     * @param totalSlots Toplam zaman dilimi sayisi.
     * @param courseRooms Frontend icin uretilen ders-oda metinlerini tutacak Hash Map.
     * @post courseRooms haritasina her ders icin oda atama metni (std::string) eklenir.
     */
    // BURASI GÜNCELLENDİ: Frontend'e veri taşımak için courseRooms eklendi
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

                std::string assignedRoomStr = "";  // HTML'E GİDECEK YAZI

                std::cout << "  -> Course: " << course->id << " (" << studentsRemaining
                          << " Students) -> Assigned Room(s): ";

                for (int r = 0; r < localRooms.size() && studentsRemaining > 0; r++) {
                    Room room = localRooms.get(r);

                    if (room.capacity > 0) {
                        if (room.capacity >= studentsRemaining) {
                            std::cout << room.id << " [" << studentsRemaining << " seats reserved] ";
                            assignedRoomStr += room.id;  // İSMİ KAYDET
                            room.capacity -= studentsRemaining;
                            studentsRemaining = 0;
                        } else {
                            std::cout << room.id << " [" << room.capacity << " seats merged] + ";
                            assignedRoomStr += room.id + " + ";  // İSMİ KAYDET
                            studentsRemaining -= room.capacity;
                            room.capacity = 0;
                        }
                        localRooms.set(r, room);
                    }
                }

                if (studentsRemaining > 0) {
                    std::cout << " -> !!! [CAPACITY INSUFFICIENT]";
                    assignedRoomStr = "Yetersiz Kapasite / Atanamadi";
                    assignedRoomStr += " (Yer Kalmadi!)";
                }
                std::cout << "\n";

                // SONUCU HARİTAYA EKLE (Frontend buradan okuyacak)
                courseRooms.insert(course->id, assignedRoomStr);
            }
        }
    }
};

#endif  // SCHEDULER_H