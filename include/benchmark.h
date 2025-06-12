#pragma once

#include <random>
#include <memory>
#include "graph.h"

class Benchmark {
private:
    std::mt19937 rng;
    std::uniform_real_distribution<double> weight_dist;

public:
    Benchmark();
    Graph generateRandomGraph(int V, double density);
    Graph generateCompleteGraph(int V);
    double measureTime(Graph& g, std::unique_ptr<ParallelizationStrategy> strategy);
    void benchmarkDifferentSizes();
    void benchmarkDifferentDensities();
    void benchmarkCompleteGraphs();
    void runComprehensiveBenchmark();
};
