#pragma once

#include <random>
#include <memory>
#include "graph.h"
///@brief class which implement benchmarking
class Benchmark {
private:
    std::mt19937 rng;
    std::uniform_real_distribution<double> weight_dist;

public:
    Benchmark();
    /**
     * @brief the method which generate random graph with entered density
     * @param V number of vertices
     * @param density the density
     * @return created random graph
     */
    Graph generateRandomGraph(int V, double density);
    /**
     * @brief the method which generate complete graph
     * @param V number of vertices
     * @return created complete graph
     */
    Graph generateCompleteGraph(int V);
    /**
     * @brief the method for measuring time needed for algorithm with input strategy
     * @param g graph
     * @param strategy strategy of computation
     * @return time in ms
     */
    double measureTime(Graph& g, std::unique_ptr<ParallelizationStrategy> strategy);
    ///@brief benchmark which run measurement functions with different sizes but the same density
    void benchmarkDifferentSizes();
    ///@brief benchmark which run measurement functions with different densities but the same size
    void benchmarkDifferentDensities();
    ///@brief benchmark which run measurement functions for complete graph of different sizes
    void benchmarkCompleteGraphs();
    ///@brief benchmark which run measurement functions for different variations
    void runComprehensiveBenchmark();
};
