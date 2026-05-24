#ifndef ROOM_H
#define ROOM_H

#include <string>

/**
 * @brief Sinavlarin yapilacagi odalari (siniflari) temsil eden yapi.
 */
struct Room {
  std::string id; ///< Odanin kimligi (Orn: "D101")
  int capacity;   ///< Odanin alabilecegi maksimum ogrenci sayisi

  Room() : id(""), capacity(0) {}
  Room(std::string roomId, int cap) : id(roomId), capacity(cap) {}
};

#endif // ROOM_H