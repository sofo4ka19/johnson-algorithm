#include <gtest/gtest.h>
#include "../include/fibonacci_heap.h"
#include "../include/constants.h"

class FibonacciHeapTest : public ::testing::Test {
protected:
    void SetUp() override {
        heap = new FibonacciHeap();
    }

    void TearDown() override {
        delete heap;
    }

    FibonacciHeap* heap;
};

TEST_F(FibonacciHeapTest, EmptyHeap) {
    EXPECT_TRUE(heap->isEmpty());
    EXPECT_FALSE(heap->contains(0));
}

TEST_F(FibonacciHeapTest, InsertSingleElement) {
    heap->insert(0, 5.0);
    EXPECT_FALSE(heap->isEmpty());
    EXPECT_TRUE(heap->contains(0));
}

TEST_F(FibonacciHeapTest, ExtractMinSingleElement) {
    heap->insert(0, 5.0);
    auto result = heap->extractMin();
    EXPECT_EQ(result.first, 0);
    EXPECT_EQ(result.second, 5.0);
    EXPECT_TRUE(heap->isEmpty());
}

TEST_F(FibonacciHeapTest, ExtractMinMultipleElements) {
    heap->insert(0, 10.0);
    heap->insert(1, 5.0);
    heap->insert(2, 15.0);

    auto result1 = heap->extractMin();
    EXPECT_EQ(result1.first, 1);
    EXPECT_EQ(result1.second, 5.0);

    auto result2 = heap->extractMin();
    EXPECT_EQ(result2.first, 0);
    EXPECT_EQ(result2.second, 10.0);

    auto result3 = heap->extractMin();
    EXPECT_EQ(result3.first, 2);
    EXPECT_EQ(result3.second, 15.0);

    EXPECT_TRUE(heap->isEmpty());
}

TEST_F(FibonacciHeapTest, DecreaseKey) {
    heap->insert(0, 10.0);
    heap->insert(1, 20.0);
    heap->insert(2, 30.0);

    heap->decreaseKey(2, 5.0);

    auto result = heap->extractMin();
    EXPECT_EQ(result.first, 2);
    EXPECT_EQ(result.second, 5.0);
}

TEST_F(FibonacciHeapTest, DecreaseKeyInvalidOperation) {
    heap->insert(0, 10.0);

    // Спроба збільшити ключ - нічого не повинно статися
    heap->decreaseKey(0, 20.0);

    auto result = heap->extractMin();
    EXPECT_EQ(result.first, 0);
    EXPECT_EQ(result.second, 10.0);
}

TEST_F(FibonacciHeapTest, ExtractMinFromEmptyHeap) {
    auto result = heap->extractMin();
    EXPECT_EQ(result.first, -1);
    EXPECT_EQ(result.second, INF);
}