#include <iostream>
#include <thread>
#include "../include/graph.h"

int main() {
    int V;
    std::cout<<"enter number of vertices in graph"<<std::endl;
    std::cin>>V;
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

    char option='+';
    while(option=='+'){
        int src, dest;
        double weight;
        std::cout<<"enter the number of source, destination vertices and weight of the edge"<< std::endl;
        std::cin >> src >> dest >> weight;
        g.addEdge(src,dest,weight);
        std::cout << "to add an edge enter '+', to exit enter any other symbol" << std::endl;
        std::cin >> option;
    }
    g.printMatrix();
    return 0;
}