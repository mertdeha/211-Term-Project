#ifndef HASHMAP_H
#define HASHMAP_H
#include <type_traits>
#include <string>
#include "LinkedList.h" // Kendi LinkedList yapını kullanabilmek için ekliyoruz

// 1. Key-Value Çifti Yapısı
template <typename K, typename V>
struct KeyValuePair {
    K key;
    V value;

    KeyValuePair() : key(K()), value(V()) {}
    KeyValuePair(K k, V v) : key(k), value(v) {}
    bool operator==(const KeyValuePair& other) const {
    return key == other.key;  
}
};


//HashMap Sınıf Tanımı
template <typename K, typename V>
class HashMap {
    static_assert(std::is_same<K, std::string>::value, 
              "HashMap only supports string keys.");
private:

    LinkedList<KeyValuePair<K, V>>* table; 
    size_t tableSize;

    //String ID'leri sayısal indekse çeviren fonksiyon
    size_t getIndex(const K& key) const {
        size_t hash = 0;
        size_t prime = 31;
        for (char c : key) {
            hash = (hash * prime + c) % tableSize;
        }
        return hash;
    }

public:
    HashMap(size_t size = 1009) : tableSize(size) {
        table = new LinkedList<KeyValuePair<K, V>>[tableSize];
    }

    ~HashMap() {
        delete[] table;
    }

    void insert(const K& key, const V& value) {
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

bool remove(const K& key) {
    size_t index = getIndex(key);
    auto current = table[index].getHead();
    while (current != nullptr) {
        if (current->data.key == key) {
            table[index].remove(current->data);  // LinkedList'ine göre uyarla
            return true;
        }
        current = current->next;
    }
    return false;
}

    V* get(const K& key) const{
        size_t index = getIndex(key);
        
        //Kendi LinkedList yapının düğüm (Node) ismine göre burayı güncellemelisin.
        auto current = table[index].getHead(); 
        while (current != nullptr) {
            if (current->data.key == key) {
                return &(current->data.value);
            }
            current = current->next;
        }
        return nullptr;
    }

    bool contains(const K& key) const{
        return get(key) != nullptr;
    }
    
    HashMap(const HashMap&) = delete;
    HashMap& operator=(const HashMap&) = delete;
};

#endif // HASHMAP_H