#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <iostream>
#include <stdexcept>

template <typename T> class DynamicArray {
private:
  T *data;
  int capacity;
  int currentSize;

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
  DynamicArray() {
    capacity = 2;
    currentSize = 0;
    data = new T[capacity];
  }

  ~DynamicArray() { delete[] data; }

  void push_back(T element) {
    if (currentSize == capacity) {
      resize();
    }
    data[currentSize] = element;
    currentSize++;
  }

  T get(int index) const {
    if (index >= 0 && index < currentSize) {
      return data[index];
    }
    throw std::out_of_range("Gecersiz indeks!");
  }

  // Var olan elemani degistirmek/guncellemek icin eklendi
  void set(int index, T value) {
    if (index >= 0 && index < currentSize) {
      data[index] = value;
      return;
    }
    throw std::out_of_range("Gecersiz indeks!");
  }

  // Welsh-Powell algoritmasi icin eklendi (Cakisma sayisina gore BUYUKTEN
  // KUCUGE siralar)
  void sortDesc() {
    for (int i = 1; i < currentSize; i++) {
      T key = data[i];
      int j = i - 1;

      // Kararsizligi onlemek adina pointer/nesne kontroluyle edges.getSize()
      // degerine bakiyoruz
      while (j >= 0 && data[j]->edges.getSize() < key->edges.getSize()) {
        data[j + 1] = data[j];
        j = j - 1;
      }
      data[j + 1] = key;
    }
  }

  int size() const { return currentSize; }
};

#endif