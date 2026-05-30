#ifndef HASHMAP_H
#define HASHMAP_H
#include <string>
#include <type_traits>

#include "LinkedList.h"

/**
 * @file HashMap.h
 * @brief Custom data structure providing fast lookups based on Key-Value pairs.
 */

/**
 * @brief Structure holding the Key and Value pair to be stored inside the HashMap.
 * @tparam K Key type (restricted to std::string).
 * @tparam V Value type.
 */
template <typename K, typename V>
struct KeyValuePair {
    K key;
    V value;

    KeyValuePair() : key(K()), value(V()) {}
    KeyValuePair(K k, V v) : key(k), value(v) {}
    bool operator==(const KeyValuePair &other) const {
        return key == other.key;
    }
};

/**
 * @brief Hash Map class that provides O(1) lookup speeds using string keys instead of array indices.
 * Implemented as a replacement for STL std::map. It handles collisions using the chaining method.
 * @tparam K Key type.
 * @tparam V Value type.
 */
template <typename K, typename V>
class HashMap {
    static_assert(std::is_same<K, std::string>::value, "HashMap only supports string keys.");

   private:
    LinkedList<KeyValuePair<K, V>> *table;  ///< Table used for chaining (linked lists)
    size_t tableSize;                       ///< Size of the hash table

    /**
     * @brief Hash Function that converts a string key into a mathematical array index.
     * @param key The string key.
     * @return size_t The corresponding index in the array.
     */
    size_t getIndex(const K &key) const {
        size_t hash = 0;
        size_t prime = 31;
        for (char c : key) {
            hash = (hash * prime + c) % tableSize;
        }
        return hash;
    }

   public:
    /**
     * @brief Constructs an empty Hash Map with a specified size.
     * @param size The size of the hash table.
     */
    HashMap(size_t size = 1009) : tableSize(size) {
        table = new LinkedList<KeyValuePair<K, V>>[tableSize];
    }

    /**
     * @brief Destructor. Clears the HashMap and its internal lists from memory.
     */
    ~HashMap() {
        delete[] table;
    }

    /**
     * @brief Inserts a new Key-Value pair into the map or updates the value if the key already exists.
     * @param key The string key to be added or updated.
     * @param value The value corresponding to the key.
     */
    void insert(const K &key, const V &value) {
        size_t index = getIndex(key);
        auto current = table[index].getHead();
        while (current != nullptr) {
            if (current->data.key == key) {
                current->data.value = value;
                return;
            }
            current = current->next;
        }
        table[index].insert(KeyValuePair<K, V>(key, value));
    }

    /**
     * @brief Deletes the element with the specified key from the map.
     * @param key The key to be deleted.
     * @return bool True if the element was successfully deleted, false if it wasn't found.
     */
    bool remove(const K &key) {
        size_t index = getIndex(key);
        auto current = table[index].getHead();
        while (current != nullptr) {
            if (current->data.key == key) {
                table[index].remove(current->data);
                return true;
            }
            current = current->next;
        }
        return false;
    }

    /**
     * @brief Returns the value corresponding to the given key.
     * @param key The key to look up.
     * @return V* The memory address of the value, or nullptr if not found.
     */
    V *get(const K &key) const {
        size_t index = getIndex(key);

        auto current = table[index].getHead();
        while (current != nullptr) {
            if (current->data.key == key) {
                return &(current->data.value);
            }
            current = current->next;
        }
        return nullptr;
    }

    /**
     * @brief Checks whether the key exists inside the map.
     * @param key The key to search for.
     * @return bool True if it exists, false otherwise.
     */
    bool contains(const K &key) const {
        return get(key) != nullptr;
    }

    // Copy semantics are disabled to prevent unintended object copying
    HashMap(const HashMap &) = delete;
    HashMap &operator=(const HashMap &) = delete;
};

#endif  // HASHMAP_H