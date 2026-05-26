#ifndef HASHMAP_H
#define HASHMAP_H
#include <string>
#include <type_traits>

#include "LinkedList.h"

/**
 * @file HashMap.h
 * @brief Anahtar-Deger (Key-Value) tabanli hizli arama yapilmasini saglayan ozel veri yapisi.
 */

/**
 * @brief HashMap icerisinde tutulacak Anahtar ve Deger ikilisini tutan yapi.
 * @tparam K Anahtar tipi (Sadece std::string).
 * @tparam V Deger tipi.
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
 * @brief Dizi indeksi yerine string anahtarlari kullanarak O(1) arama hizi sunan Hash Map sinifi.
 * STL std::map yerine yazilmistir. Chaining yontemi ile cakismalari cozer.
 * @tparam K Anahtar (Key) tipi.
 * @tparam V Deger (Value) tipi.
 */
template <typename K, typename V>
class HashMap {
    static_assert(std::is_same<K, std::string>::value, "HashMap only supports string keys.");

   private:
    LinkedList<KeyValuePair<K, V>> *table;  ///< Chaining (bagli liste) icin tablo
    size_t tableSize;                       ///< Hash tablosunun boyutu

    /**
     * @brief String anahtari alip matematiksel bir dizi indeksine donusturen Hash Fonksiyonu.
     * @param key String anahtar.
     * @return size_t Dizideki indeks karsiligi.
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
     * @brief Belirtilen boyutta bos bir Hash Map olusturur.
     * @param size Hash tablosunun boyutu.
     */
    HashMap(size_t size = 1009) : tableSize(size) {
        table = new LinkedList<KeyValuePair<K, V>>[tableSize];
    }

    /**
     * @brief HashMap'i ve icindeki listeleri hafizadan temizler.
     */
    ~HashMap() {
        delete[] table;
    }

    /**
     * @brief Haritaya yeni bir Anahtar-Deger cifti ekler veya var olan anahtari gunceller.
     * @param key Eklenecek string anahtar.
     * @param value Anahtara karsilik gelen deger.
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
     * @brief Belirtilen anahtara sahip elemani haritadan siler.
     * @param key Silinecek anahtar.
     * @return bool Eger silindiyse true, bulunamadiysa false.
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
     * @brief Anahtari verilen degeri dondurur.
     * @param key Aranacak anahtar.
     * @return V* Degerin bellek adresi. Bulunamazsa nullptr.
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
     * @brief Anahtarin map icinde olup olmadigini kontrol eder.
     * @param key Aranacak anahtar.
     * @return bool Varsa true, yoksa false.
     */
    bool contains(const K &key) const {
        return get(key) != nullptr;
    }

    // Kopya yapici metodlari kapatildi (Istenmeyen kopya olusumunu onlemek icin)
    HashMap(const HashMap &) = delete;
    HashMap &operator=(const HashMap &) = delete;
};

#endif  // HASHMAP_H