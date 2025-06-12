#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>
#include <thread>
#include "../include/graph.h"
#include "../include/benchmark.h"

enum class ModeType {
    Interactive,
    Benchmark
};
///@brief the basic class for work with modes
class Mode {
public:
    virtual ~Mode() = default;
    virtual void run() = 0;
};

///@brief the class which implements interactive mode
class InteractiveMode : public Mode {
public:
    void run() override {
        int V;
        std::cout << "Enter number of vertices in graph: ";
        std::cin >> V;
        Graph g(V);

        char strategy_choice;
        std::cout << "Choose execution strategy: 's' for sequential, 'p' for parallel: ";
        std::cin >> strategy_choice;

        if (strategy_choice == 'p') {
            g.setStrategy(std::make_unique<ParallelDijkstraStrategy>());
            std::cout << "Using parallel strategy" << std::endl;
        } else {
            g.setStrategy(std::make_unique<SequentialStrategy>());
            std::cout << "Using sequential strategy" << std::endl;
        }

        char option = '+';
        while (option == '+') {
            int src, dest;
            double weight;
            std::cout << "Enter source, destination vertices and weight of the edge: ";
            std::cin >> src >> dest >> weight;
            g.addEdge(src, dest, weight);
            std::cout << "To add an edge enter '+', to exit enter any other symbol: ";
            std::cin >> option;
        }
        g.printMatrix();
    }
};

///@brief the class which implements benchmark mode
class BenchmarkMode : public Mode {
public:
    void run() override {
        std::cout << "Running benchmarks..." << std::endl;
        Benchmark benchmark;
        benchmark.runComprehensiveBenchmark();
    }
};

///@brief the class which implements factory pattern for modes
class ModeFactory {
public:
    static std::unique_ptr<Mode> createMode(ModeType mode) {
        switch (mode) {
            case ModeType::Interactive:
                return std::make_unique<InteractiveMode>();
            case ModeType::Benchmark:
                return std::make_unique<BenchmarkMode>();
            default:
                throw std::invalid_argument("Invalid ModeType");
        }
    }
};
// Конвертація рядка в enum
ModeType parseModeType(const std::string& modeStr) {
    if (modeStr == "interactive" || modeStr == "1") {
        return ModeType::Interactive;
    } else if (modeStr == "benchmark" || modeStr == "2") {
        return ModeType::Benchmark;
    } else {
        throw std::invalid_argument("Unknown mode: " + modeStr);
    }
}

int main(int argc, char* argv[]) {
    try {
        ModeType mode;

        if (argc > 1) {
            mode = parseModeType(argv[1]);
        } else {
            std::cout << "Choose mode:\n";
            std::cout << "1. Interactive\n";
            std::cout << "2. Benchmark\n";
            std::cout << "Enter number (1 or 2): ";
            std::string choice;
            std::cin >> choice;
            mode = parseModeType(choice);
        }

        auto selectedMode = ModeFactory::createMode(mode);
        selectedMode->run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "Usage: " << argv[0] << " [interactive|test|benchmark]" << std::endl;
        return 1;
    }

    return 0;
}