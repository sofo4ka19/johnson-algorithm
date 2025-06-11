#include <gtest/gtest.h>
#include "../include/graph.h"
#include "../include/constants.h"

class GraphTest : public ::testing::Test {
protected:
    void SetUp() override {
        graph = new Graph(4);
    }

    void TearDown() override {
        delete graph;
    }

    Graph* graph;
};

TEST_F(GraphTest, GraphCreation) {
    EXPECT_EQ(graph->getV(), 4);
}

TEST_F(GraphTest, AddEdge) {
    graph->addEdge(0, 1, 5.0);
    const auto& adj = graph->getAdj();
    EXPECT_FALSE(adj[0].empty());
    EXPECT_EQ(adj[0].front().dest, 1);
    EXPECT_EQ(adj[0].front().weight, 5.0);
}

TEST_F(GraphTest, BellmanFordNoNegativeCycle) {
    graph->addEdge(0, 1, -1);
    graph->addEdge(1, 2, -3);
    graph->addEdge(2, 3, 2);
    graph->addEdge(3, 1, 1);

    std::vector<double> dist;
    bool result = graph->bellmanFord(0, dist);

    EXPECT_TRUE(result);
    EXPECT_EQ(dist[0], 0);
    EXPECT_EQ(dist[1], -1);
    EXPECT_EQ(dist[2], -4);
    EXPECT_EQ(dist[3], -2);
}

TEST_F(GraphTest, BellmanFordWithNegativeCycle) {
    graph->addEdge(0, 1, 1);
    graph->addEdge(1, 2, -3);
    graph->addEdge(2, 3, 2);
    graph->addEdge(3, 1, -5);

    std::vector<double> dist;
    bool result = graph->bellmanFord(0, dist);

    EXPECT_FALSE(result);
}

TEST_F(GraphTest, DijkstraSimple) {
    graph->addEdge(0, 1, 2);
    graph->addEdge(0, 2, 4);
    graph->addEdge(1, 2, 1);
    graph->addEdge(1, 3, 7);
    graph->addEdge(2, 3, 3);

    std::vector<double> dist;
    graph->dijkstraWithFibHeap(0, dist);

    EXPECT_EQ(dist[0], 0);
    EXPECT_EQ(dist[1], 2);
    EXPECT_EQ(dist[2], 3);
    EXPECT_EQ(dist[3], 6);
}

TEST_F(GraphTest, JohnsonSequential) {
    graph->setStrategy(std::make_unique<SequentialStrategy>());

    graph->addEdge(0, 1, 3);
    graph->addEdge(0, 2, 8);
    graph->addEdge(0, 3, -4);
    graph->addEdge(1, 3, 1);
    graph->addEdge(2, 1, 4);
    graph->addEdge(3, 1, 7);
    graph->addEdge(3, 2, -5);

    auto result = graph->johnson();

    EXPECT_EQ(result.size(), 4);
    EXPECT_EQ(result[0].size(), 4);
    EXPECT_EQ(result[0][0], 0);
    EXPECT_EQ(result[0][1], 1);
    EXPECT_EQ(result[0][2], -3);
    EXPECT_EQ(result[0][3], -4);
}

TEST_F(GraphTest, JohnsonParallel) {
    graph->setStrategy(std::make_unique<ParallelDijkstraStrategy>());

    graph->addEdge(0, 1, 3);
    graph->addEdge(0, 2, 8);
    graph->addEdge(0, 3, -4);
    graph->addEdge(1, 3, 1);
    graph->addEdge(2, 1, 4);
    graph->addEdge(3, 1, 7);
    graph->addEdge(3, 2, -5);

    auto result = graph->johnson();

    EXPECT_EQ(result.size(), 4);
    EXPECT_EQ(result[0].size(), 4);
    EXPECT_EQ(result[0][0], 0);
    EXPECT_EQ(result[0][1], 1);
    EXPECT_EQ(result[0][2], -3);
    EXPECT_EQ(result[0][3], -4);
}

TEST_F(GraphTest, InvalidEdge) {
    testing::internal::CaptureStdout();
    graph->addEdge(-1, 0, 1);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output.empty());
}