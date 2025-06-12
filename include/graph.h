#pragma once

#include <vector>
#include <list>
#include <limits>
#include <memory>
#include <mutex>
#include "fibonacci_heap.h"
#include "thread_pool.h"

///@brief struct for the graph edge
struct Edge {
    int dest;
    double weight;

    Edge(int _dest, double _weight);
};

// Forward declaration
class Graph;

///@brief class for Strategy Pattern for different strategies of calculations
class ParallelizationStrategy {
public:
    virtual ~ParallelizationStrategy() = default;
    /**
     * @brief A virtual method for implementing execution
     * @param graph with type Graph
     * @return matrix of the shortest ways between vertex
     */
    virtual std::vector<std::vector<double>> execute(Graph& graph) = 0;
};

///@brief class for implementing sequential strategy of computation
class SequentialStrategy : public ParallelizationStrategy {
public:
    std::vector<std::vector<double>> execute(Graph& graph) override;
};

///@brief class for implementing parallel strategy of computation
class ParallelDijkstraStrategy : public ParallelizationStrategy {
private:
    ///@brief the field which contains the number of threads
    size_t thread_count;
public:
    ///@brief constructor of the class which set thread count
    ParallelDijkstraStrategy(size_t threads = 0)
            : thread_count(threads == 0 ? std::thread::hardware_concurrency() : threads) {}
    ///@brief function which implement the opportunity to change the number of threads after creating the object of the class
    void setThreadCount(size_t threads) { thread_count = threads; }
    ///@return thread count
    size_t getThreadCount() const { return thread_count; }
    std::vector<std::vector<double>> execute(Graph& graph) override;
};

///@brief class for the graph implementation
class Graph {
private:
    int V;  // Кількість вершин
    std::vector<std::list<Edge>> adj;  // Список суміжності
    mutable std::mutex graph_mutex;
    std::unique_ptr<ParallelizationStrategy> strategy;

public:
    Graph(int V);
    Graph(const Graph&) = delete;
    Graph& operator=(const Graph&) = delete;
    Graph(Graph&& other) noexcept :
            V(other.V),
            adj(std::move(other.adj)),
            strategy(std::move(other.strategy))
    // mutex ініціалізується за замовчуванням
    {}

    Graph& operator=(Graph&& other) noexcept {
        if (this != &other) {
            V = other.V;
            adj = std::move(other.adj);
            strategy = std::move(other.strategy);
            // mutex не потрібно переміщати
        }
        return *this;
    }
    /**
     * @brief function for adding an edge to the graph
     * @param src the vertex from which is the edge
     * @param dest the vertex where is the end of the edge
     * @param weight the weight of the edge
     */
    void addEdge(int src, int dest, double weight);

    ///@brief setting the strategy of computation
    void setStrategy(std::unique_ptr<ParallelizationStrategy> newStrategy);

    /**
     * @brief Bellman-Ford algorithm implementation
     * @param src the vertex which is the start, in our case it ias fictional
     * @param dist the array which represent distances from that vertex, is used by reference
     * @return bool value if the graph contains negative cycles
     */
    bool bellmanFord(int src, std::vector<double>& dist);

    /**
     * @brief Dijkstra algorithm using Fibonacci heap
     * @param src the vertex for which we search the shortest paths
     * @param dist the array which represent distances from that vertex, is used by reference
     */
    void dijkstraWithFibHeap(int src, std::vector<double>& dist);

    /**
     * @brief Johnson's algorithm
     * @return the matrix which represent the shortest paths between vertices
     */
    std::vector<std::vector<double>> johnson();

    ///@brief function for printing matrix
    void printMatrix();

    // Геттери
    int getV() const;
    const std::vector<std::list<Edge>>& getAdj() const;
    std::vector<std::list<Edge>>& getAdjMutable();
};
