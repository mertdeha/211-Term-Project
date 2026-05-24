#ifndef COURSE_H
#define COURSE_H

#include "../data_structures/LinkedList.h" // Kendi yazdigimiz listeyi dahil ettik
#include <string>

/**
 * @brief Bir dersi ve o derse kayitli ogrencileri temsil eden yapi.
 */
struct Course {
  std::string id;   ///< Dersin kodu (Orn: "CSE101")
  std::string name; ///< Dersin adi (Orn: "Intro to CS")

  /**
   * @brief Derse kayitli ogrencilerin ID'lerini tutan bagli liste.
   * STL std::vector KULLANMAK YASAK oldugu icin kendi listemizi kullaniyoruz.
   */
  LinkedList<std::string> enrolledStudents;

  Course() : id(""), name("") {}
  Course(std::string courseId, std::string courseName)
      : id(courseId), name(courseName) {}

  /**
   * @brief Derse yeni bir ogrenci ekler.
   * @param studentId Eklenecek ogrencinin ID'si
   */
  void addStudent(std::string studentId) { enrolledStudents.insert(studentId); }
};

#endif // COURSE_H