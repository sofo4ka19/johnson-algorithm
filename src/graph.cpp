#include "../include/graph.h"
#include "../include/constants.h"
#include "../include/thread_pool.h"
#include <iostream>
#include <algorithm>
#include <thread>
#include <future>
#include <vector>
#include <list>

// Edge implementation
Edge::Edge(int _dest, double _weight) : dest(_dest), weight(_weight) {}

// Graph implementation
Graph::Graph(int V) : V(V), adj(V) {}

void Graph::addEdge(int src, int dest, double weight) {
    if (src < 0 || src >= V || dest < 0 || dest >= V) {
        std::cout << "Error: Invalid vertex indices. Vertices must be in range [0, " << V-1 << "]" << std::endl;
        return;
    }
    adj[src].push_back(Edge(dest, weight));
}

void Graph::setStrategy(std::unique_ptr<ParallelizationStrategy> newStrategy) {
    strategy = std::move(newStrategy);
}

bool Graph::bellmanFord(int src, std::vector<double>& dist) {
    dist.assign(V, INF);
    dist[src] = 0;

    // Релаксація ребер V-1 разів
    for (int i = 0; i < V - 1; i++) {
        bool updated = false;

        for (int u = 0; u < V; u++) {
            if (dist[u] == INF) continue;

            for (const Edge &e: adj[u]) {
                int v = e.dest;
                double weight = e.weight;

                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    updated = true;
                }
            }
        }

        if (!updated) break;
    }

    // Перевіряємо наявність циклів з від'ємною вагою
    for (int u = 0; u < V; u++) {
        if (dist[u] == INF) continue;

        for (const Edge& e : adj[u]) {
            int v = e.dest;
            double weight = e.weight;

            if (dist[u] + weight < dist[v]) {
                return false;
            }
        }
    }

    return true;
}

void Graph::dijkstraWithFibHeap(int src, std::vector<double>& dist) {
    dist.assign(V, INF);
    dist[src] = 0;

    FibonacciHeap heap;
    std::vector<bool> processed(V, false);

    for (int v = 0; v < V; v++) {
        heap.insert(v, dist[v]);
    }

    while (!heap.isEmpty()) {
        auto [u, dist_u] = heap.extractMin();

        if (processed[u]) continue;
        processed[u] = true;

        // Релаксація ребер
        for (const Edge& e : adj[u]) {
            int v = e.dest;
            double weight = e.weight;

            if (!processed[v] && dist[u] != INF && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                if (heap.contains(v)) {
                    heap.decreaseKey(v, dist[v]);
                }
            }
        }
    }
}

std::vector<std::vector<double>> Graph::johnson() {
    return strategy->execute(*this);
}

void Graph::printMatrix() {
    std::vector<std::vector<double>> dist = johnson();
    std::cout << "Matrix of the shortest paths:" << std::endl;
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (dist[i][j] == INF)
                std::cout << "INF\t";
            else
                std::cout << dist[i][j] << "\t";
        }
        std::cout << std::endl;
    }
}

int Graph::getV() const {
    return V;
}

const std::vector<std::list<Edge>>& Graph::getAdj() const {
    return adj;
}

std::vector<std::list<Edge>>& Graph::getAdjMutable() {
    return adj;
}

// SequentialStrategy implementation
std::vector<std::vector<double>> SequentialStrategy::execute(Graph& graph) {
    int V = graph.getV();

    // Створюємо копію оригінального графу
    Graph originalGraph(V);
    for (int u = 0; u < V; u++) {
        for (const Edge& e : graph.getAdj()[u]) {
            originalGraph.addEdge(u, e.dest, e.weight);
        }
    }

    // Створюємо новий граф з додатковою вершиною s
    Graph g(V + 1);
    for (int u = 0; u < V; u++) {
        for (const Edge& e : originalGraph.getAdj()[u]) {
            g.addEdge(u, e.dest, e.weight);
        }
    }

    // Додаємо ребра з нульовою вагою від s до всіх вершин
    for (int v = 0; v < V; v++) {
        g.addEdge(V, v, 0);
    }

    // Запускаємо Беллмана-Форда з вершини s
    std::vector<double> h;
    if (!g.bellmanFord(V, h)) {
        std::cout << "Граф містить цикл з від'ємною вагою!" << std::endl;
        return std::vector<std::vector<double>>(V, std::vector<double>(V, INF));
    }

    // Створюємо граф з перетвореними вагами
    Graph transformedGraph(V);
    for (int u = 0; u < V; u++) {
        for (const Edge& e : originalGraph.getAdj()[u]) {
            double newWeight = e.weight + h[u] - h[e.dest];
            transformedGraph.addEdge(u, e.dest, newWeight);
        }
    }

    // Послідовно запускаємо Дейкстру з кожної вершини
    std::vector<std::vector<double>> dist(V, std::vector<double>(V, INF));

    for (int src = 0; src < V; src++) {
        transformedGraph.dijkstraWithFibHeap(src, dist[src]);

        // Перетворюємо відстані назад
        for (int v = 0; v < V; v++) {
            if (dist[src][v] != INF) {
                dist[src][v] = dist[src][v] - h[src] + h[v];
            }
        }
    }

    return dist;
}

// ParallelDijkstraStrategy implementation
std::vector<std::vector<double>> ParallelDijkstraStrategy::execute(Graph& graph) {
    int V = graph.getV();

    // Створюємо копію оригінального графу
    Graph originalGraph(V);
    for (int u = 0; u < V; u++) {
        for (const Edge& e : graph.getAdj()[u]) {
            originalGraph.addEdge(u, e.dest, e.weight);
        }
    }

    // Створюємо новий граф з додатковою вершиною
    Graph g(V + 1);
    for (int u = 0; u < V; u++) {
        for (const Edge& e : originalGraph.getAdj()[u]) {
            g.addEdge(u, e.dest, e.weight);
        }
    }

    for (int v = 0; v < V; v++) {
        g.addEdge(V, v, 0);
    }

    // Беллман-Форд
    std::vector<double> h;
    if (!g.bellmanFord(V, h)) {
        std::cout << "Граф містить цикл з від'ємною вагою!" << std::endl;
        return std::vector<std::vector<double>>(V, std::vector<double>(V, INF));
    }

    // Створюємо граф з перетвореними вагами
    Graph transformedGraph(V);
    for (int u = 0; u < V; u++) {
        for (const Edge& e : originalGraph.getAdj()[u]) {
            double newWeight = e.weight + h[u] - h[e.dest];
            transformedGraph.addEdge(u, e.dest, newWeight);
        }
    }

    // Паралельний запуск Дейкстри
    std::vector<std::vector<double>> dist(V, std::vector<double>(V, INF));
    std::vector<std::future<void>> futures;

    ThreadPool pool(std::thread::hardware_concurrency());

    for (int src = 0; src < V; src++) {
        futures.push_back(pool.enqueue([&transformedGraph, src, &dist, &h, V]() {
            transformedGraph.dijkstraWithFibHeap(src, dist[src]);

            // Перетворення відстаней назад
            for (int v = 0; v < V; v++) {
                if (dist[src][v] != INF) {
                    dist[src][v] = dist[src][v] - h[src] + h[v];
                }
            }
        }));
    }

    for (auto& future : futures) {
        future.wait();
    }

    return dist;
}