#ifndef GRAPH_H
#define GRAPH_H

#include "../data_structures/HashMap.h"    // HashMap'i dahil ettik
#include "../data_structures/LinkedList.h" // LinkedList'i dahil ettik
#include <iostream>
#include <string>

struct Vertex {
  std::string id;
  LinkedList<std::string> edges;
  Vertex *next;

  Vertex(std::string vertexId) : id(vertexId), next(NULL) {}
};

class Graph {
private:
  Vertex *head;
  int numVertices;
  HashMap<std::string, Vertex *> vertexMap; // Hizli erisim icin map ekledik

public:
  Graph() : head(NULL), numVertices(0) {}

  ~Graph() { clear(); }

  void addVertex(std::string id) {
    if (getVertex(id) != NULL)
      return;

    Vertex *newVertex = new Vertex(id);
    if (head == NULL) {
      head = newVertex;
    } else {
      Vertex *current = head;
      while (current->next != NULL) {
        current = current->next;
      }
      current->next = newVertex;
    }
    numVertices++;
    vertexMap.insert(id, newVertex); // Map'e de kaydediyoruz
  }

  void addEdge(std::string src, std::string dest) {
    Vertex *v1 = getVertex(src);
    Vertex *v2 = getVertex(dest);

    if (v1 != NULL && v2 != NULL) {
      v1->edges.insert(dest);
      v2->edges.insert(src);
    }
  }

  // Artik O(N) degil, O(1) hizinda arama yapiyor!
  Vertex *getVertex(std::string id) const {
    Vertex **found = vertexMap.get(id);
    if (found != NULL) {
      return *found;
    }
    return NULL;
  }

  // Algoritmanin tum sinavlari donmesi icin head pointer'ini veren fonksiyon
  Vertex *getHead() const { return head; }
  int getNumVertices() const { return numVertices; }

  void printGraph() const {
    Vertex *current = head;
    while (current != NULL) {
      std::cout << "[" << current->id << "] sunlarla cakisiyor: ";

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

  void clear() {
    Vertex *current = head;
    while (current != NULL) {
      Vertex *nextVertex = current->next;
      delete current;
      current = nextVertex;
    }
    head = NULL;
    numVertices = 0;
  }

  int getDegree(std::string id) const {
    Vertex *v = getVertex(id);
    if (v != NULL) {
      return v->edges.getSize(); // LinkedList'inin getSize() fonksiyonunu kullaniyoruz
    }
    return 0;
  }
  
  const LinkedList<std::string>& getNeighbors(std::string id) const {
    Vertex *v = getVertex(id);
    if (v != NULL) {
      return v->edges; 
    }
    static const LinkedList<std::string> emptyList;
    return emptyList;
  }
};

#endif // GRAPH_H