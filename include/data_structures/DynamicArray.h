#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <iostream>
#include <stdexcept>

/**
 * @file DynamicArray.h
 * @brief Custom dynamic array implementation to be used instead of STL vector.
 */

/**
 * @brief Dynamic array class capable of automatically expanding its capacity.
 * @tparam T The type of data to be stored in the array.
 */
template <typename T>
class DynamicArray {
   private:
    T *data;          ///< Dynamic memory block where elements are stored
    int capacity;     ///< Current maximum capacity of the array
    int currentSize;  ///< Current number of elements inside the array

    /**
     * @brief Doubles the capacity of the array.
     * @post Capacity expands, and old data is copied into the new memory allocation.
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
     * @brief Default constructor. Allocates an initial space for 2 elements.
     */
    DynamicArray() {
        capacity = 2;
        currentSize = 0;
        data = new T[capacity];
    }

    /**
     * @brief Destructor. Deallocates memory to prevent leaks.
     */
    ~DynamicArray() {
        delete[] data;
    }

    /**
     * @brief Adds a new element to the end of the array. Resizes if necessary.
     * @param element The data to be added.
     */
    void push_back(T element) {
        if (currentSize == capacity) {
            resize();
        }
        data[currentSize] = element;
        currentSize++;
    }

    /**
     * @brief Returns the element at the specified index.
     * @param index The index of the requested element.
     * @return T The data at the specified index.
     * @throws std::out_of_range If the index is out of bounds.
     */
    T get(int index) const {
        if (index >= 0 && index < currentSize) {
            return data[index];
        }
        throw std::out_of_range("Invalid index!");
    }

    /**
     * @brief Updates the element at the specified index.
     * @param index The index of the element to be updated.
     * @param value The new data value.
     * @throws std::out_of_range If the index is out of bounds.
     */
    void set(int index, T value) {
        if (index >= 0 && index < currentSize) {
            data[index] = value;
            return;
        }
        throw std::out_of_range("Invalid index!");
    }

    /**
     * @brief Sorts courses in descending order by conflict count (degree) for the Welsh-Powell algorithm.
     * @post Courses within the array are ordered from highest number of edges to lowest.
     */
    void sortDesc() {
        for (int i = 1; i < currentSize; i++) {
            T key = data[i];
            int j = i - 1;

            // Checking edges.getSize() via pointer/object validation to guarantee stability
            while (j >= 0 && data[j]->edges.getSize() < key->edges.getSize()) {
                data[j + 1] = data[j];
                j = j - 1;
            }
            data[j + 1] = key;
        }
    }

    /**
     * @brief Returns the current number of elements in the array.
     * @return int The element count.
     */
    int size() const {
        return currentSize;
    }
};

#endif  // DYNAMIC_ARRAY_H