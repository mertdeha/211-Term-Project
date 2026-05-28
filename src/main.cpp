/**
 * @file main.cpp
 * @brief PROJ-16 Sinav Cizelgeleme Sistemi main dosyası.
 * * Bu dosya; verilerin JSON'dan okunmasi, cakisim grafinin olusturulmasi,
 * graf boyama (zaman dilimi atamasi), kutu paketleme (oda atamasi) ve
 * web arayuzunun uretilmesi adimlarini sirasiyla calistirir.
 * * @authors Mert Deha Karayel - Serkan Can Erdem - Selin Yaslan
 * @date Mayis 2026
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
    std::cout << "--- PROJ-16: Sinav Cizelgeleme Sistemi---\n";
    std::cout << "=========================================================================\n\n";

    DynamicArray<Course *> allCourses;
    DynamicArray<Room> allRooms;

    // Varsayilan test dosyasi
    std::string inputFile = "data/input_sample.json";
    if (argc > 1) {
        inputFile = argv[1];  // Terminalden girilen dosyayi al
    }

    std::cout << "\n>>> ADIM 1: JSON Dosyasindan Veriler Okunuyor...\n";
    Parser::parseJSON(inputFile, allCourses);
    Parser::parseRooms(inputFile, allRooms);

    std::cout << "[INFO] Hafizadaki Toplam Ders Sayisi: " << allCourses.size() << "\n";
    std::cout << "[INFO] Hafizadaki Toplam Oda Sayisi: " << allRooms.size() << "\n";

    // Eger veri tamamen boşsa sistemi güvenle kapat.
    if (allCourses.size() == 0) {
        std::cout << "\n[UYARI] Okunan veri setinde hic ders bulunamadi! Program guvenle sonlandiriliyor.\n";

        // Eski HTML dosyasinin ekranda yaniltici bilgi gostermemesi icin uzerine "Bos" uyarisi yaziyoruz:
        std::ofstream htmlFile("frontend/exam_schedule.html");
        if (htmlFile.is_open()) {
            htmlFile << "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>No Data</title></head>";
            htmlFile
                << "<body style='font-family:sans-serif; text-align:center; padding:50px; background-color:#f8f9fa;'>";
            htmlFile << "<h2 style='color:#e74c3c;'>Girdi dosyasinda hic ders bulunamadi (Veya dosya acilamadi).</h2>";
            htmlFile << "<p style='color:#7f8c8d;'>Gosterilecek bir sinav programi yok.</p>";
            htmlFile << "</body></html>";
            htmlFile.close();
            std::cout << "[FRONTEND] Eski HTML dosyasi temizlendi (Bos ekran uretildi).\n";
        }

        return 0;
    }

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
    HashMap<std::string, std::string> courseRooms;
    Scheduler::assignRooms(allCourses, allRooms, courseToSlotMap, totalAvailableSlots, courseRooms);

    std::cout << "\n>>> ADIM 5: Web Arayuzu Uretiliyor...\n";
    Frontend::generateWebInterface(allCourses, courseToSlotMap, courseRooms, totalAvailableSlots);

    std::cout << "\n=========================================================================\n";
    std::cout << "--- Tum Modullerin Testi Basariyla Tamamlandi ------\n";
    std::cout << "=========================================================================\n";

    for (int i = 0; i < allCourses.size(); i++) {
        delete allCourses.get(i);
    }

    return 0;
}