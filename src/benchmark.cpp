#include "../include/benchmark.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <thread>
#include <vector>

Benchmark::Benchmark() : rng(42), weight_dist(-10.0, 100.0) {}

Graph Benchmark::generateRandomGraph(int V, double density) {
    Graph g(V);
    std::uniform_int_distribution<int> vertex_dist(0, V-1);

    int total_edges = static_cast<int>(V * V * density);

    for (int i = 0; i < total_edges; i++) {
        int src = vertex_dist(rng);
        int dest = vertex_dist(rng);
        if (src != dest) {
            double weight = weight_dist(rng);
            g.addEdge(src, dest, weight);
        }
    }

    return g;
}

Graph Benchmark::generateCompleteGraph(int V) {
    Graph g(V);

    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (i != j) {
                double weight = weight_dist(rng);
                g.addEdge(i, j, weight);
            }
        }
    }

    return g;
}

double Benchmark::measureTime(Graph& g, std::unique_ptr<ParallelizationStrategy> strategy) {
    g.setStrategy(std::move(strategy));

    auto start = std::chrono::high_resolution_clock::now();
    auto result = g.johnson();
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    return duration.count() / 1000.0; // повертаємо в мілісекундах
}

void Benchmark::benchmarkDifferentSizes() {
    std::cout << "Performance Test: Different Graph Sizes (density=0.3)" << std::endl;
    std::cout << "Hardware concurrency: " << std::thread::hardware_concurrency() << " threads" << std::endl;
    std::cout << std::string(65, '-') << std::endl;

    std::vector<int> sizes = {25, 50, 75, 100, 150, 200};

    for (int V : sizes) {
        auto g1 = generateRandomGraph(V, 0.3);
        auto g2 = generateRandomGraph(V, 0.3);

        std::cout << "Testing " << V << " vertices..." << std::endl;

        double seq_time = measureTime(g1, std::make_unique<SequentialStrategy>());
        std::cout << "  Sequential: " << std::fixed << std::setprecision(1) << seq_time << " ms" << std::endl;

        double par_time = measureTime(g2, std::make_unique<ParallelDijkstraStrategy>());
        std::cout << "  Parallel:   " << std::fixed << std::setprecision(1) << par_time << " ms" << std::endl;

        double speedup = seq_time / par_time;
        std::cout << "  Speedup:    " << std::fixed << std::setprecision(2) << speedup << "x" << std::endl;
        std::cout << std::endl;
    }
}

void Benchmark::benchmarkDifferentDensities() {
    std::cout << "Performance Test: Different Graph Densities (V=100)" << std::endl;
    std::cout << std::string(65, '-') << std::endl;

    std::vector<double> densities = {0.1, 0.2, 0.3, 0.5, 0.7, 0.9};

    for (double density : densities) {
        auto g1 = generateRandomGraph(100, density);
        auto g2 = generateRandomGraph(100, density);

        std::cout << "Testing density " << std::fixed << std::setprecision(1) << density << "..." << std::endl;

        double seq_time = measureTime(g1, std::make_unique<SequentialStrategy>());
        std::cout << "  Sequential: " << std::fixed << std::setprecision(1) << seq_time << " ms" << std::endl;

        double par_time = measureTime(g2, std::make_unique<ParallelDijkstraStrategy>());
        std::cout << "  Parallel:   " << std::fixed << std::setprecision(1) << par_time << " ms" << std::endl;

        double speedup = seq_time / par_time;
        std::cout << "  Speedup:    " << std::fixed << std::setprecision(2) << speedup << "x" << std::endl;
        std::cout << std::endl;
    }
}

void Benchmark::benchmarkCompleteGraphs() {
    std::cout << "Performance Test: Complete Graphs" << std::endl;
    std::cout << std::string(65, '-') << std::endl;

    std::vector<int> sizes = {15, 25, 35, 45, 55};

    for (int V : sizes) {
        auto g1 = generateCompleteGraph(V);
        auto g2 = generateCompleteGraph(V);

        std::cout << "Testing complete graph with " << V << " vertices..." << std::endl;

        double seq_time = measureTime(g1, std::make_unique<SequentialStrategy>());
        std::cout << "  Sequential: " << std::fixed << std::setprecision(1) << seq_time << " ms" << std::endl;

        double par_time = measureTime(g2, std::make_unique<ParallelDijkstraStrategy>());
        std::cout << "  Parallel:   " << std::fixed << std::setprecision(1) << par_time << " ms" << std::endl;

        double speedup = seq_time / par_time;
        std::cout << "  Speedup:    " << std::fixed << std::setprecision(2) << speedup << "x" << std::endl;
        std::cout << std::endl;
    }
}

void Benchmark::runComprehensiveBenchmark() {
    std::cout << "Comprehensive Performance Test" << std::endl;
    std::cout << std::string(80, '=') << std::endl;

    std::vector<int> sizes = {25, 50, 75, 100, 150, 200};
    std::vector<double> densities = {0.1, 0.3, 0.5, 0.7};

    for (int V : sizes) {
        std::cout << "\nGraph Size: " << V << " vertices" << std::endl;
        std::cout << std::string(40, '-') << std::endl;

        for (double density : densities) {
            auto g1 = generateRandomGraph(V, density);
            auto g2 = generateRandomGraph(V, density);

            double seq_time = measureTime(g1, std::make_unique<SequentialStrategy>());
            double par_time = measureTime(g2, std::make_unique<ParallelDijkstraStrategy>());

            std::cout << "Density " << std::fixed << std::setprecision(1) << density
                      << ": Sequential=" << std::setprecision(1) << seq_time << "ms"
                      << ", Parallel=" << par_time << "ms"
                      << ", Speedup=" << std::setprecision(2) << (seq_time/par_time) << "x"
                      << std::endl;
        }
    }
}