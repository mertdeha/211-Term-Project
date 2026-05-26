#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <string>

#include "../data_structures/HashMap.h"
#include "../data_structures/LinkedList.h"

/**
 * @file Graph.h
 * @brief Cakisim grafini (Conflict Graph) olusturan yapi.
 */

/**
 * @brief Graftaki bir dugumu (Vertex) temsil eder.
 */
struct Vertex {
    std::string id;                 ///< Dugumun kimligi (Orn: Ders Kodu)
    LinkedList<std::string> edges;  ///< Bu dugumun bagli oldugu komsulari (kenarlar)
    Vertex *next;                   ///< Bir sonraki dugumu isaret eden pointer

    /**
     * @brief Vertex nesnesi olusturur.
     * @param vertexId Dugume verilecek ID.
     */
    Vertex(std::string vertexId) : id(vertexId), next(NULL) {}
};

/**
 * @brief Ders cakisimlarini modellemek icin kullanilan Yonlendirilmemis Graf (Undirected Graph) sinifi.
 */
class Graph {
   private:
    Vertex *head;                              ///< Graftaki ilk dugum
    int numVertices;                           ///< Toplam dugum sayisi
    HashMap<std::string, Vertex *> vertexMap;  ///< Dugum arama hizini O(1) yapmak icin HashMap

   public:
    /**
     * @brief Bos bir graf olusturur.
     */
    Graph() : head(NULL), numVertices(0) {}

    /**
     * @brief Grafi ve tum dugumleri hafizadan temizler.
     */
    ~Graph() {
        clear();
    }

    /**
     * @brief Grafa yeni bir dugum (Ders) ekler.
     * @param id Eklenecek dugumun ID'si.
     */
    void addVertex(std::string id) {
        if (getVertex(id) != NULL) return;

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
        vertexMap.insert(id, newVertex);
    }

    /**
     * @brief Iki dugum arasinda kenar (edge) olusturur.
     * @param src Baslangic dugumu.
     * @param dest Hedef dugumu.
     */
    void addEdge(std::string src, std::string dest) {
        Vertex *v1 = getVertex(src);
        Vertex *v2 = getVertex(dest);

        if (v1 != NULL && v2 != NULL) {
            v1->edges.insert(dest);
            v2->edges.insert(src);
        }
    }

    /**
     * @brief ID'si verilen dugumu HashMap uzerinden hizlica (O(1)) bulup dondurur.
     * @param id Aranacak dugumun ID'si.
     * @return Vertex* Dugumun bellek adresi (Bulunamazsa NULL).
     */
    Vertex *getVertex(std::string id) const {
        Vertex **found = vertexMap.get(id);
        if (found != NULL) {
            return *found;
        }
        return NULL;
    }

    /**
     * @brief Graftaki ilk dugumu dondurur.
     * @return Vertex* Grafin head pointer'i.
     */
    Vertex *getHead() const {
        return head;
    }

    /**
     * @brief Graftaki toplam dugum sayisini dondurur.
     * @return int Dugum sayisi.
     */
    int getNumVertices() const {
        return numVertices;
    }

    /**
     * @brief Grafin yapisini (hangi dersin kimlerle cakistigini) terminale yazdirir.
     */
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

    /**
     * @brief Graftaki tum verileri siler, hafizayi bosaltir.
     */
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

    /**
     * @brief Belirtilen dugumun bagli oldugu komsularinin sayisini (Degree) doner.
     * @param id Derecesi hesaplanacak dugumun ID'si.
     * @return int Komsuluk derecesi.
     */
    int getDegree(std::string id) const {
        Vertex *v = getVertex(id);
        if (v != NULL) {
            return v->edges.getSize();
        }
        return 0;
    }

    /**
     * @brief Bir dugumun komsularinin (edges) bagli listesini dondurur.
     * @param id Komsu listesi alinacak dugum ID'si.
     * @return const LinkedList<std::string>& Komsularin listesi.
     */
    const LinkedList<std::string> &getNeighbors(std::string id) const {
        Vertex *v = getVertex(id);
        if (v != NULL) {
            return v->edges;
        }
        static const LinkedList<std::string> emptyList;
        return emptyList;
    }
};

#endif  // GRAPH_H