#ifndef COURSE_H
#define COURSE_H

#include <string>

#include "../data_structures/LinkedList.h"

/**
 * @file Course.h
 * @brief Sinav cizelgeleme sistemindeki bir dersi temsil eder.
 */

/**
 * @brief Bir dersin kimlik bilgilerini ve o derse kayitli ogrencileri tutan sinif.
 *
 * Sinav cakisimlarini tespit edebilmek icin her dersin benzersiz bir ID'si, adi ve
 * o derse kayitli olan ogrencilerin bir bagli listesi (LinkedList) tutulur.
 * STL std::vector KULLANMAK YASAK oldugu icin kendi listemizi kullaniyoruz.
 */
struct Course {
    std::string id;    ///< Dersin kodu (Orn: "CSE101")
    std::string name;  ///< Dersin adi (Orn: "Intro to CS")

    /**
     * @brief Derse kayitli ogrencilerin ID'lerini tutan bagli liste.
     */
    LinkedList<std::string> enrolledStudents;

    /**
     * @brief Bos bir Course nesnesi olusturur.
     */
    Course() : id(""), name("") {}

    /**
     * @brief Yeni bir Course (Ders) nesnesi olusturur.
     * @param courseId Dersin kodu.
     * @param courseName Dersin tam adi.
     */
    Course(std::string courseId, std::string courseName) : id(courseId), name(courseName) {}

    /**
     * @brief Derse yeni bir ogrenci ekler.
     * @param studentId Eklenecek ogrencinin adi veya ID'si.
     * @post Ogrenci listesine yeni bir dugum (node) eklenmis olur.
     */
    void addStudent(std::string studentId) {
        enrolledStudents.insert(studentId);
    }
};

#endif  // COURSE_H