#ifndef ROOM_H
#define ROOM_H

#include <string>

/**
 * @file Room.h
 * @brief Sinavlarin yapilacagi odalari (siniflari) temsil eden sinif.
 */

/**
 * @brief Sinavlarin atanacagi fiziksel odalari temsil eden yapi.
 */
struct Room {
    std::string id;  ///< Odanin kimligi (Orn: "D101")
    int capacity;    ///< Odanin alabilecegi maksimum ogrenci sayisi

    /**
     * @brief Bos bir oda nesnesi olusturur.
     */
    Room() : id(""), capacity(0) {}

    /**
     * @brief Yeni bir oda nesnesi olusturur.
     * @param roomId Odanin adi veya kodu.
     * @param cap Odanin ogrenci kapasitesi.
     */
    Room(std::string roomId, int cap) : id(roomId), capacity(cap) {}
};

#endif  // ROOM_H