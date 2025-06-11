#include <cmath>
#include <vector>
#include "../include/fibonacci_heap.h"
#include "../include/constants.h"

FibNode::FibNode(int v, double k) : vertex(v), key(k), parent(nullptr), child(nullptr),
                           left(this), right(this), degree(0), mark(false) {}

void FibonacciHeap::addRoot(FibNode* x){
    if(min==nullptr){
        x->left=x;
        x->right=x;
        min=x;
        return;
    }
    x->right = min;
    x->left = min->left;
    min->left->right = x;
    min->left = x;

    x->parent = nullptr;
    x->mark = false;

    if(x->key < min->key){
        min=x;
    }
}

void FibonacciHeap::link(FibNode* y, FibNode* x) {
    // Видаляємо y з кореневого списку
    y->left->right = y->right;
    y->right->left = y->left;

    // Робимо y дочірнім для x
    if (x->child == nullptr) {
        x->child = y;
        y->right = y;
        y->left = y;
    } else {
        y->right = x->child;
        y->left = x->child->left;
        x->child->left->right = y;
        x->child->left = y;
    }

    y->parent = x;
    x->degree++;
    y->mark = false;
}

void FibonacciHeap::consolidate() {
    int max_degree = static_cast<int>(log2(n) * 2);
    std::vector<FibNode*> A(max_degree + 1, nullptr);

    // Створюємо список усіх коренів
    std::vector<FibNode*> roots;
    if (min == nullptr) return;

    FibNode* current = min;
    do {
        roots.push_back(current);
        current = current->right;
    } while (current != min);

    // Обробляємо кожен корінь
    for (FibNode* root : roots) {
        FibNode* x = root;
        int d = x->degree;

        // Доки існує дерево того ж степеня
        while (A[d] != nullptr) {
            FibNode* y = A[d];
            if (x->key > y->key) {
                std::swap(x, y);
            }

            link(y, x);
            A[d] = nullptr;
            d++;
        }

        A[d] = x;
    }
    // Перебудовуємо список коренів
    min = nullptr;
    for (int i = 0; i <= max_degree; i++) {
        if (A[i] != nullptr) {
            addRoot(A[i]);
        }
    }
}

void FibonacciHeap::cut(FibNode* x, FibNode* y) {
    // Видаляємо x з дочірнього списку y
    if (x->right == x) {
        y->child = nullptr;
    } else {
        x->right->left = x->left;
        x->left->right = x->right;
        if (y->child == x) {
            y->child = x->right;
        }
    }

    y->degree--;

    // Додаємо x до кореневого списку
    addRoot(x);
}

void FibonacciHeap::cascadingCut(FibNode* y) {
    FibNode* z = y->parent;
    if (z != nullptr) {
        if (!y->mark) {
            y->mark = true;
        } else {
            cut(y, z);
            cascadingCut(z);
        }
    }
}

FibonacciHeap::FibonacciHeap() : min(nullptr), n(0) {}
FibonacciHeap::~FibonacciHeap() {
    // Очищаємо пам'ять від усіх вузлів
    for (auto& pair : nodes) {
        delete pair.second;
    }
}

bool FibonacciHeap::isEmpty() const {
    return min == nullptr;
}

void FibonacciHeap::insert(int vertex, double key) {
    FibNode* node = new FibNode(vertex, key);
    nodes[vertex] = node;

    addRoot(node);

    n++;
}

void FibonacciHeap::decreaseKey(int vertex, double newKey) {
    FibNode* x = nodes[vertex];
    if (newKey > x->key) {
        return;  // Новий ключ більший, нічого не робимо
    }

    x->key = newKey;
    FibNode* y = x->parent;

    if (y != nullptr && x->key < y->key) {
        cut(x, y);
        cascadingCut(y);
    }

    if (x->key < min->key) {
        min = x;
    }
}

std::pair<int, double> FibonacciHeap::extractMin() {
    FibNode* z = min;
    if (z == nullptr) {
        return {-1, INF};
    }

    // Додаємо всіх дітей z до кореневого списку
    if (z->child != nullptr) {
        FibNode* child = z->child;
        FibNode* temp = child;
        do {
            FibNode* next = temp->right;
            addRoot(temp);
            temp = next;
        } while (temp != child);
    }

    // Видаляємо z з кореневого списку
    z->left->right = z->right;
    z->right->left = z->left;

    if (z == z->right) {
        min = nullptr;
    } else {
        min = z->right;
        consolidate();
    }

    n--;

    std::pair<int, double> result = {z->vertex, z->key};
    nodes.erase(z->vertex);
    delete z;
    return result;
}
bool FibonacciHeap::contains(int vertex) const {
    return nodes.find(vertex) != nodes.end();
}