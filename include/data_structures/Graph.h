#ifndef GRAPH_H
#define GRAPH_H

#include "LinkedList.h" // Kendi listemizi dahil ediyoruz
#include <iostream>
#include <string>

/**
 * @brief Graftaki her bir dugumu (dersi/sinavi) temsil eder.
 */
struct Vertex {
  std::string id;                ///< Dersin kimligi (Orn: CSE101)
  LinkedList<std::string> edges; ///< Bu dersle cakisan diger derslerin listesi
  Vertex *next; ///< Graf icindeki bir sonraki dugumu isaret eder

  /**
   * @brief Vertex kurucu metodu
   */
  Vertex(std::string vertexId) : id(vertexId), next(NULL) {}
};

/**
 * @brief Sinavlar arasi cakismani modelleyen yonsuz graf (Undirected Graph)
 * sinifi.
 */
class Graph {
private:
  Vertex *head;    ///< Graftaki ilk dugumun pointer'i
  int numVertices; ///< Toplam dugum sayisi

public:
  Graph() : head(NULL), numVertices(0) {}

  ~Graph() { clear(); }

  /**
   * @brief Grafa yeni bir dugum (ders) ekler.
   * @param id Eklenecek dersin kodu
   */
  void addVertex(std::string id) {
    // Eger dugum zaten varsa ekleme
    if (getVertex(id) != NULL)
      return;

    Vertex *newVertex = new Vertex(id);
    if (head == NULL) {
      head = newVertex;
    } else {
      // Listenin sonuna ekle
      Vertex *current = head;
      while (current->next != NULL) {
        current = current->next;
      }
      current->next = newVertex;
    }
    numVertices++;
  }

  /**
   * @brief Iki dugum arasina kenar ceker (Cakismayi belirtir).
   * Yonsuz graf oldugu icin A'dan B'ye ve B'den A'ya baglanti ekler.
   */
  void addEdge(std::string src, std::string dest) {
    Vertex *v1 = getVertex(src);
    Vertex *v2 = getVertex(dest);

    // Iki dugum de grafta bulunmali
    if (v1 != NULL && v2 != NULL) {
      v1->edges.insert(dest);
      v2->edges.insert(src);
    }
  }

  /**
   * @brief ID'si verilen dugumu bulur ve dondurur.
   */
  Vertex *getVertex(std::string id) const {
    Vertex *current = head;
    while (current != NULL) {
      if (current->id == id) {
        return current;
      }
      current = current->next;
    }
    return NULL; // Bulunamazsa null doner
  }

  /**
   * @brief Grafin tamamini (dugumleri ve komsularini) ekrana yazdirir.
   */
  void printGraph() const {
    Vertex *current = head;
    while (current != NULL) {
      std::cout << "[" << current->id << "] sunlarla cakisiyor: ";

      // Komsu listesini geziyoruz
      Node<std::string> *edgeCurrent = current->edges.getHead();
      if (edgeCurrent == NULL) {
        std::cout << "Hicbiri";
      }
      while (edgeCurrent != NULL) {
        std::cout << edgeCurrent->data << " ";
        edgeCurrent = edgeCurrent->next;
      }
      std::cout << std::endl;

      current = current->next;
    }
  }

  /**
   * @brief Bellek sizintilarini onlemek icin grafi temizler.
   */
  void clear() {
    Vertex *current = head;
    while (current != NULL) {
      Vertex *nextVertex = current->next;
      delete current; // Vertex silindiginde icindeki LinkedList'in de
                      // destructoru otomatik calisir!
      current = nextVertex;
    }
    head = NULL;
    numVertices = 0;
  }
};

#endif // GRAPH_H