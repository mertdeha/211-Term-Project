#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <iostream>
#include <stdexcept>

/**
 * @file DynamicArray.h
 * @brief STL vector yerine kullanilacak kendi yazdigimiz dinamik dizi yapisi.
 */

/**
 * @brief Boyutu otomatik olarak genisleyebilen dinamik dizi sinifi.
 * @tparam T Dizide tutulacak verinin tipi.
 */
template <typename T>
class DynamicArray {
   private:
    T *data;          ///< Verilerin tutuldugu dinamik hafiza blogu
    int capacity;     ///< Dizinin su anki maksimum kapasitesi
    int currentSize;  ///< Dizideki mevcut eleman sayisi

    /**
     * @brief Dizi kapasitesini 2 katina cikarir.
     * @post Kapasite genisler, eski veriler yeni alana kopyalanir.
     */
    void resize() {
        capacity = capacity * 2;
        T *newData = new T[capacity];

        for (int i = 0; i < currentSize; i++) {
            newData[i] = data[i];
        }

        delete[] data;
        data = newData;
    }

   public:
    /**
     * @brief Varsayilan kurucu metod (Constructor). 2 elemanlik alan ayirir.
     */
    DynamicArray() {
        capacity = 2;
        currentSize = 0;
        data = new T[capacity];
    }

    /**
     * @brief Yikici metod (Destructor). Hafiza sizintilarini engeller.
     */
    ~DynamicArray() {
        delete[] data;
    }

    /**
     * @brief Dizinin sonuna yeni eleman ekler. Gerekirse boyutu buyutur.
     * @param element Eklenecek veri.
     */
    void push_back(T element) {
        if (currentSize == capacity) {
            resize();
        }
        data[currentSize] = element;
        currentSize++;
    }

    /**
     * @brief Belirtilen indeksteki elemani dondurur.
     * @param index Alinmak istenen elemanin indeksi.
     * @return T Indeksteki veri.
     * @throws std::out_of_range Indeks sinirlarin disindaysa.
     */
    T get(int index) const {
        if (index >= 0 && index < currentSize) {
            return data[index];
        }
        throw std::out_of_range("Gecersiz indeks!");
    }

    /**
     * @brief Belirtilen indeksteki elemani gunceller.
     * @param index Guncellenecek elemanin indeksi.
     * @param value Yeni veri.
     * @throws std::out_of_range Indeks sinirlarin disindaysa.
     */
    void set(int index, T value) {
        if (index >= 0 && index < currentSize) {
            data[index] = value;
            return;
        }
        throw std::out_of_range("Gecersiz indeks!");
    }

    /**
     * @brief Welsh-Powell algoritmasi icin dersleri cakisma sayisina gore (derece) azalan siralar.
     * @post Dizi icerisindeki dersler, edges sayisi en cok olandan en aza dogru siralanir.
     */
    void sortDesc() {
        for (int i = 1; i < currentSize; i++) {
            T key = data[i];
            int j = i - 1;

            // Kararsizligi onlemek adina pointer/nesne kontroluyle edges.getSize() degerine bakiyoruz
            while (j >= 0 && data[j]->edges.getSize() < key->edges.getSize()) {
                data[j + 1] = data[j];
                j = j - 1;
            }
            data[j + 1] = key;
        }
    }

    /**
     * @brief Dizideki mevcut eleman sayisini dondurur.
     * @return int Eleman sayisi.
     */
    int size() const {
        return currentSize;
    }
};

#endif