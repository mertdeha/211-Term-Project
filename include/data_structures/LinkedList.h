#ifndef LINKEDLIST_H
#define LINKEDLIST_H

/**
 * @file LinkedList.h
 * @brief Custom Linked List data structure implemented instead of using the STL.
 */

/**
 * @brief Structure representing a linked list node (Node).
 * @tparam T The type of data to be stored inside the node.
 */
template <typename T>
struct Node {
    T data;      ///< The data contained within the node
    Node *next;  ///< Pointer targeting the next node

    /**
     * @brief Node constructor.
     * @param val Initial value to be assigned to the node.
     */
    Node(T val) : data(val), next(nullptr) {}
};

/**
 * @brief Dynamic, singly linked list class.
 * To be used as a replacement for STL std::vector or std::list.
 * @tparam T The type of data to be stored in the list.
 */
template <typename T>
class LinkedList {
   private:
    Node<T> *head;  ///< Points to the first element of the list
    Node<T> *tail;  ///< Points to the last element of the list (for fast insertion)
    int size;       ///< Total number of nodes inside the list

   public:
    /**
     * @brief Constructs an empty linked list.
     */
    LinkedList() : head(nullptr), tail(nullptr), size(0) {}

    /**
     * @brief Destructor. Prevents memory leaks by clearing all nodes from
     * the heap when the object is destroyed.
     */
    ~LinkedList() {
        clear();
    }

    /**
     * @brief Appends a new element to the end of the list. O(1) time complexity.
     * @param value The data to be added.
     */
    void insert(T value) {
        Node<T> *newNode = new Node<T>(value);
        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        size++;
    }

    /**
     * @brief Removes the specified data from the list.
     * @param value The data value to be deleted.
     * @return bool True if successfully deleted, false if the element was not found.
     */
    bool remove(const T &value) {
        Node<T> *current = head;
        Node<T> *prev = nullptr;

        while (current != nullptr) {
            if (current->data == value) {
                if (prev == nullptr)  // If the head node is being deleted
                    head = current->next;
                else
                    prev->next = current->next;

                if (current == tail)  // If the tail node is being deleted
                    tail = prev;

                delete current;
                size--;
                return true;
            }
            prev = current;
            current = current->next;
        }
        return false;
    }

    /**
     * @brief Returns the element at the front of the list (essential for iteration).
     * @return Node<T>* Pointer to the first node.
     */
    Node<T> *getHead() const {
        return head;
    }

    /**
     * @brief Returns the number of elements currently in the list.
     * @return int The list size.
     */
    int getSize() const {
        return size;
    }

    /**
     * @brief Deletes all elements in the list and deallocates the heap memory.
     */
    void clear() {
        Node<T> *current = head;
        while (current != nullptr) {
            Node<T> *nextNode = current->next;
            delete current;
            current = nextNode;
        }
        head = nullptr;
        tail = nullptr;
        size = 0;
    }
};

#endif  // LINKEDLIST_H