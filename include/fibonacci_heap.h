#pragma once
#include <unordered_map>

///@brief the structure for the node of the fibonacci heap
struct FibNode {
    int vertex;
    double key;
    FibNode* parent;
    FibNode* child;
    FibNode* left;
    FibNode* right;
    int degree;
    bool mark;

    FibNode(int v, double k);
};

///@brief class for implementing fibonacci heap
class FibonacciHeap {
private:
    FibNode* min;
    int n;
    std::unordered_map<int, FibNode*> nodes;

    //додати до коренів
    void addRoot(FibNode* x);
    ///@brief uniting 2 trees of the same power
    void link(FibNode* y, FibNode* x);

    // Консолідація дерев
    void consolidate();

    /**
     * @brief function for cutting the subtree
     * @param x the subtree
     * @param y the tree
     */
    void cut(FibNode* x, FibNode* y);

    // Рекурсивне відокремлення
    void cascadingCut(FibNode* y);

public:
    FibonacciHeap();

    ~FibonacciHeap();

    // Перевіряємо, чи піраміда порожня
    bool isEmpty() const ;

    // Додаємо новий вузол
    void insert(int vertex, double key);

    // Зменшуємо ключ вузла
    void decreaseKey(int vertex, double newKey);

    /**
     * @brief deleting and returning the min element
     * @return pair of vertex and key of the min element
     */
    std::pair<int, double> extractMin() ;

    // Перевіряємо, чи містить піраміда вершину
    bool contains(int vertex) const ;
};