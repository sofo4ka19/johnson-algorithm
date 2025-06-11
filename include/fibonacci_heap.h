#pragma once
#include <unordered_map>

// Структура для представлення вузла піраміди Фібоначчі
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

// Клас для піраміди Фібоначчі
class FibonacciHeap {
private:
    FibNode* min;
    int n;
    std::unordered_map<int, FibNode*> nodes;

    //додати до коренів
    void addRoot(FibNode* x);
    // Об'єднати два дерева одного степеня
    void link(FibNode* y, FibNode* x);

    // Консолідація дерев
    void consolidate();

    // Відокремлення піддерева
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

    // Видаляємо і повертаємо мінімальний елемент
    std::pair<int, double> extractMin() ;

    // Перевіряємо, чи містить піраміда вершину
    bool contains(int vertex) const ;
};