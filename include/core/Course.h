#ifndef COURSE_H
#define COURSE_H

#include <string>

#include "../data_structures/LinkedList.h"

/**
 * @file Course.h
 * @brief Represents a course in the exam timetabling system.
 */

/**
 * @brief Class that stores a course's identification details and its enrolled students.
 *
 * To detect exam conflicts, each course holds a unique ID, a name, and a linked
 * list (LinkedList) of students enrolled in that course.
 * Using STL std::vector IS FORBIDDEN, so we are using our custom list implementation.
 */
struct Course {
    std::string id;    ///< Course code (e.g., "CSE101")
    std::string name;  ///< Course name (e.g., "Intro to CS")

    /**
     * @brief Linked list holding the IDs of students enrolled in the course.
     */
    LinkedList<std::string> enrolledStudents;

    /**
     * @brief Constructs an empty Course object.
     */
    Course() : id(""), name("") {}

    /**
     * @brief Constructs a new Course object with specific details.
     * @param courseId The code of the course.
     * @param courseName The full name of the course.
     */
    Course(std::string courseId, std::string courseName) : id(courseId), name(courseName) {}

    /**
     * @brief Enrolls a new student into the course.
     * @param studentId The name or ID of the student to be added.
     * @post A new node is added to the enrolled students list.
     */
    void addStudent(std::string studentId) {
        enrolledStudents.insert(studentId);
    }
};

#endif  // COURSE_H