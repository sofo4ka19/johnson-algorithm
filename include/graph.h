#pragma once

#include <vector>
#include <list>
#include <limits>
#include <memory>
#include <mutex>
#include "fibonacci_heap.h"
#include "thread_pool.h"

// Структура для представлення ребра графа
struct Edge {
    int dest;
    double weight;

    Edge(int _dest, double _weight);
};

// Forward declaration
class Graph;

// Strategy Pattern для різних стратегій паралелізації
class ParallelizationStrategy {
public:
    virtual ~ParallelizationStrategy() = default;
    virtual std::vector<std::vector<double>> execute(Graph& graph) = 0;
};

// Послідовна стратегія
class SequentialStrategy : public ParallelizationStrategy {
public:
    std::vector<std::vector<double>> execute(Graph& graph) override;
};

// Паралельна стратегія з Дейкстрою
class ParallelDijkstraStrategy : public ParallelizationStrategy {
public:
    std::vector<std::vector<double>> execute(Graph& graph) override;
};

// Клас для представлення графа
class Graph {
private:
    int V;  // Кількість вершин
    std::vector<std::list<Edge>> adj;  // Список суміжності
    mutable std::mutex graph_mutex;
    std::unique_ptr<ParallelizationStrategy> strategy;

public:
    Graph(int V);

    // Додаємо ребро до графа
    void addEdge(int src, int dest, double weight);

    // Встановлюємо стратегію паралелізації
    void setStrategy(std::unique_ptr<ParallelizationStrategy> newStrategy);

    // Алгоритм Беллмана-Форда
    bool bellmanFord(int src, std::vector<double>& dist);

    // Алгоритм Дейкстри з пірамідою Фібоначчі
    void dijkstraWithFibHeap(int src, std::vector<double>& dist);

    // Алгоритм Джонсона
    std::vector<std::vector<double>> johnson();

    // Виводимо матрицю найкоротших шляхів
    void printMatrix();

    // Геттери
    int getV() const;
    const std::vector<std::list<Edge>>& getAdj() const;
    std::vector<std::list<Edge>>& getAdjMutable();
};
