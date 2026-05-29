# Technical Design and Architecture Document

## 1. Architecture Overview
The core system is structured to strictly isolate data storage representations, conflict detection mechanisms, scheduling mathematics, and presentation engines. Information streams sequentially through a defined pipeline: data ingestion (JSON parsing), in-memory representation, conflict graph generation, chronological scheduling, spatial allocation (room assignment), and finally, HTML presentation. This decoupled model ensures strict modularity where analytical phases do not possess unmediated modification rights over underlying model states.

## 2. Module Descriptions
* **Data & Configuration Parser:** Responsible for reading input datasets, parsing course, student, and room requirements, and instantiating the foundational data objects.
* **Graph Engine:** Manages the construction of the adjacency-list conflict graph by detecting shared student enrollments across distinct courses.
* **Scheduler Engine:** The core analytical module. It assigns conflict-free time slots using graph coloring heuristics and subsequently maps grouped exams to physical rooms based on capacity constraints.
* **Frontend Generator:** A decoupled presentation layer that translates the final in-memory scheduling matrices into a responsive, user-friendly HTML table.

## 3. Data Structure Choices
Due to strict pedagogical constraints prohibiting STL containers, we engineered custom data structures tailored to our specific algorithmic needs:
* **`DynamicArray`:** Replaces `std::vector`. Chosen for $O(1)$ amortized insertion and $O(1)$ indexed access. It provides contiguous memory management essential for rapidly sorting courses and rooms.
* **`LinkedList`:** Replaces `std::forward_list`. Chosen specifically to handle collision chaining in our custom hash map and to store adjacency lists in the conflict graph, ensuring $O(1)$ insertions at the head without memory fragmentation.
* **`HashMap`:** Replaces `std::map`. Chosen to map string keys (Course IDs) to structural objects. It guarantees $O(1)$ average-case lookup time, minimizing overhead during the graph coloring phase where frequent neighbor-color lookups are required.
* **`Graph` (Adjacency List):** Chosen over an adjacency matrix because university course conflicts typically form a sparse graph. It drastically reduces spatial footprint to $O(V + E)$.

## 4. Algorithm Explanations

### 4.1 Conflict Graph Construction (Intersection Scanning)
* **Mathematical Logic:** Every separate course object is registered as a vertex. The engine executes a triangular pairing intersection scan across all courses. If enrollment sequences share student IDs, a bidirectional edge is established.
* **Time Complexity:** $O(V^2 \times S^2)$ where $V$ is the number of courses and $S$ is the maximum enrollment list length.
* **Space Complexity:** $O(V + E)$ where $E$ denotes verified student conflict links.

### 4.2 Exam Slot Coloring (Welsh-Powell Heuristic)
* **Mathematical Logic:** 1. Calculate degrees and sort vertices in descending order. 2. Iterate down the sorted sequence and allocate the lowest available non-conflicting time slot index by checking immediate neighbors.
* **Time Complexity:** $O(V^2 + E)$. Sorting takes $O(V^2)$ and checking neighbors across the graph takes $O(V + E)$.
* **Space Complexity:** $O(V)$ auxiliary space to preserve color assignments.

### 4.3 Classroom Allocation (First-Fit Decreasing Bin Packing)
* **Mathematical Logic:** 1. Sort courses for the current slot by student headcount (descending). 2. Sort available classrooms by seat capacity (descending). 3. Apply First-Fit strategy. If a single room's capacity is lower than the remaining headcount, split the course across consecutive rooms.
* **Time Complexity:** $O(S \times (N^2 + R^2 + N \times R))$, where $S$ is time slots, $N$ is courses per slot, and $R$ is total physical classrooms.
* **Space Complexity:** $O(R)$ localized space requirements per slot execution loop.

## 5. Design Decisions
* **Adjacency List vs. Adjacency Matrix:** We opted for an adjacency list for the conflict graph. *Trade-off:* While edge lookups take slightly longer ($O(\text{Degree})$ vs $O(1)$), the memory savings are immense for sparse university datasets, preventing Out-Of-Memory (OOM) errors.
* **First-Fit Decreasing (FFD) vs. Best-Fit:** For room allocation, we selected FFD. *Trade-off:* Best-Fit might leave less fragmented remaining capacities in individual rooms, but FFD runs significantly faster on our custom data structures and adequately handles multi-room splitting without the overhead of continuously scanning for optimal remainders.
* **In-Memory Sorting vs. Tree Structures:** We chose to implement custom in-place sorting algorithms directly on the `DynamicArray` rather than building balanced Binary Search Trees. *Trade-off:* This increases worst-case sorting time to $O(N^2)$, but drastically simplifies memory management and eliminates complex pointer-chasing overhead.