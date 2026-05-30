#ifndef ROOM_H
#define ROOM_H

#include <string>

/**
 * @file Room.h
 * @brief Class representing the rooms (classrooms) where exams will be held.
 */

/**
 * @brief Structure representing the physical rooms where exams will be assigned.
 */
struct Room {
    std::string id;  ///< Room identifier (e.g., "D101")
    int capacity;    ///< Maximum number of students the room can accommodate

    /**
     * @brief Constructs an empty Room object.
     */
    Room() : id(""), capacity(0) {}

    /**
     * @brief Constructs a new Room object with specific details.
     * @param roomId The name or code of the room.
     * @param cap The student capacity of the room.
     */
    Room(std::string roomId, int cap) : id(roomId), capacity(cap) {}
};

#endif  // ROOM_H