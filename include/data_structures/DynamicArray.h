#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <iostream>
#include <stdexcept>

template <typename T>
class DynamicArray {
private:
    T* data;
    int capacity;
    int currentSize;

    void resize() {
        capacity = capacity * 2;
        T* newData = new T[capacity];

        // Normal for döngüsü ile kopyalama
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

    ~DynamicArray() {
        delete[] data;
    }

    void push_back(T element) {
        if (currentSize == capacity) {
            resize();
        }
        data[currentSize] = element;
        currentSize++;
    }

    T get(int index) {
        if (index >= 0 && index < currentSize) {
            return data[index];
        }
        throw std::out_of_range("Gecersiz indeks!");
    }

    int size() {
        return currentSize;
    }
};

#endif