#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <string>

#include "../data_structures/HashMap.h"
#include "../data_structures/LinkedList.h"

/**
 * @file Graph.h
 * @brief Structure that forms the Conflict Graph.
 */

/**
 * @brief Represents a vertex (node) in the graph.
 */
struct Vertex {
    std::string id;                 ///< Vertex identifier (e.g., Course Code)
    LinkedList<std::string> edges;  ///< The neighbors (edges) connected to this vertex
    Vertex *next;                   ///< Pointer targeting the next vertex

    /**
     * @brief Constructs a Vertex object.
     * @param vertexId The ID to assign to the vertex.
     */
    Vertex(std::string vertexId) : id(vertexId), next(NULL) {}
};

/**
 * @brief Undirected Graph class used to model exam conflicts.
 */
class Graph {
   private:
    Vertex *head;                              ///< First vertex in the graph
    int numVertices;                           ///< Total number of vertices
    HashMap<std::string, Vertex *> vertexMap;  ///< HashMap to ensure O(1) vertex lookup speed

   public:
    /**
     * @brief Constructs an empty graph.
     */
    Graph() : head(NULL), numVertices(0) {}

    /**
     * @brief Clears the graph and releases all vertices from memory.
     */
    ~Graph() {
        clear();
    }

    /**
     * @brief Adds a new vertex (Course) to the graph.
     * @param id The ID of the vertex to be added.
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
     * @brief Creates an edge between two vertices.
     * @param src Source vertex.
     * @param dest Destination vertex.
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
     * @brief Finds and returns the vertex with the given ID quickly (O(1)) via the HashMap.
     * @param id The ID of the vertex to search for.
     * @return Vertex* Memory address of the vertex (NULL if not found).
     */
    Vertex *getVertex(std::string id) const {
        Vertex **found = vertexMap.get(id);
        if (found != NULL) {
            return *found;
        }
        return NULL;
    }

    /**
     * @brief Returns the first vertex in the graph.
     * @return Vertex* The head pointer of the graph.
     */
    Vertex *getHead() const {
        return head;
    }

    /**
     * @brief Returns the total number of vertices in the graph.
     * @return int The vertex count.
     */
    int getNumVertices() const {
        return numVertices;
    }

    /**
     * @brief Prints the structure of the graph (which course conflicts with which courses) to the terminal.
     */
    void printGraph() const {
        Vertex *current = head;
        while (current != NULL) {
            std::cout << "[" << current->id << "] conflicts with: ";

            Node<std::string> *edgeCurrent = current->edges.getHead();
            if (edgeCurrent == NULL) {
                std::cout << "None";
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
     * @brief Deletes all data in the graph, clearing allocated memory.
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
     * @brief Returns the degree (number of connected neighbors) of the specified vertex.
     * @param id The ID of the vertex whose degree will be calculated.
     * @return int The neighborhood degree.
     */
    int getDegree(std::string id) const {
        Vertex *v = getVertex(id);
        if (v != NULL) {
            return v->edges.getSize();
        }
        return 0;
    }

    /**
     * @brief Returns the linked list of neighbors (edges) for a given vertex.
     * @param id The vertex ID whose neighbor list will be fetched.
     * @return const LinkedList<std::string>& The list of neighbors.
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