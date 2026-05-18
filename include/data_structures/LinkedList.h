#ifndef LINKEDLIST_H
#define LINKEDLIST_H

/**
 * @brief Bagli liste dugumunu (Node) temsil eden yapi.
 * @tparam T Dugumde tutulacak verinin tipi.
 */
template <typename T> struct Node {
  T data;     ///< Dugumun icindeki veri
  Node *next; ///< Bir sonraki dugumu isaret eden pointer

  /**
   * @brief Node kurucu metodu (Constructor).
   * @param val Dugume atanacak baslangic degeri.
   */
  Node(T val) : data(val), next(nullptr) {}
};

/**
 * @brief Dinamik, tek yonlu bagli liste (Singly Linked List) sinifi.
 * STL std::vector veya std::list yerine kullanilacaktir.
 * @tparam T Listede tutulacak verilerin tipi.
 */
template <typename T> class LinkedList {
private:
  Node<T> *head; ///< Listenin ilk elemanini isaret eder
  Node<T> *tail; ///< Listenin son elemanini isaret eder (Hizli ekleme icin)
  int size;      ///< Listedeki toplam dugum sayisi

public:
  /**
   * @brief Bos bir bagli liste olusturur.
   */
  LinkedList() : head(nullptr), tail(nullptr), size(0) {}

  /**
   * @brief Yikici metod (Destructor). Hafiza sizintisini (memory leak) onler.
   * Nesne silindiginde icindeki tum dugumleri hafizadan temizler.
   */
  ~LinkedList() { clear(); }

  /**
   * @brief Listenin sonuna yeni bir eleman ekler. O(1) zaman karmasikligi.
   * @param value Eklenecek veri.
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
   * @brief Listenin basindaki elemani dondurur (Iterasyon icin gereklidir).
   * @return Node<T>* Ilk dugumun pointer'i.
   */
  Node<T> *getHead() const { return head; }

  /**
   * @brief Listedeki eleman sayisini dondurur.
   * @return int Liste boyutu.
   */
  int getSize() const { return size; }

  /**
   * @brief Listedeki tum elemanlari siler ve hafizayi (heap) serbest birakir.
   */
  void clear() {
    Node<T> *current = head;
    while (current != nullptr) {
      Node<T> *nextNode = current->next;
      delete current; // Hafizayi iade et (Valgrind hatasini engeller)
      current = nextNode;
    }
    head = nullptr;
    tail = nullptr;
    size = 0;
  }
};

#endif // LINKEDLIST_H