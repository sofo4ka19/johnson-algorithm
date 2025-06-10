#include <iostream>
#include <vector>
#include <list>
#include <limits>
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <queue>
#include <utility>

// Константи
const double INF = std::numeric_limits<double>::infinity();

// Структура для представлення ребра графа
struct Edge {
    int dest;
    double weight;

    Edge(int _dest, double _weight) : dest(_dest), weight(_weight) {}
};

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

    FibNode(int v, double k) : vertex(v), key(k), parent(nullptr), child(nullptr),
                               left(this), right(this), degree(0), mark(false) {}
};

// Клас для піраміди Фібоначчі
class FibonacciHeap {
private:
    FibNode* min;
    int n;
    std::unordered_map<int, FibNode*> nodes;

    //додати до коренів
    void addRoot(FibNode* x){
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
    // Об'єднати два дерева одного степеня
    void link(FibNode* y, FibNode* x) {
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

    // Консолідація дерев
    void consolidate() {
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

    // Відокремлення піддерева
    void cut(FibNode* x, FibNode* y) {
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

    // Рекурсивне відокремлення
    void cascadingCut(FibNode* y) {
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

public:
    FibonacciHeap() : min(nullptr), n(0) {}

    ~FibonacciHeap() {
        // Очищаємо пам'ять від усіх вузлів
        for (auto& pair : nodes) {
            delete pair.second;
        }
    }

    // Перевіряємо, чи піраміда порожня
    bool isEmpty() const {
        return min == nullptr;
    }

    // Додаємо новий вузол
    void insert(int vertex, double key) {
        FibNode* node = new FibNode(vertex, key);
        nodes[vertex] = node;

        addRoot(node);

        n++;
    }

    // Зменшуємо ключ вузла
    void decreaseKey(int vertex, double newKey) {
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

    // Видаляємо і повертаємо мінімальний елемент
    std::pair<int, double> extractMin() {
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

    // Перевіряємо, чи містить піраміда вершину
    bool contains(int vertex) const {
        return nodes.find(vertex) != nodes.end();
    }
};

// Клас для представлення графа
class Graph {
private:
    int V;  // Кількість вершин
    std::vector<std::list<Edge>> adj;  // Список суміжності

public:
    Graph(int V) : V(V), adj(V) {}

    // Додаємо ребро до графа
    void addEdge(int src, int dest, double weight) {
        adj[src].push_back(Edge(dest, weight));
    }

    // Алгоритм Беллмана-Форда
    bool bellmanFord(int src, std::vector<double>& dist) {
        // Ініціалізуємо відстані
        dist.assign(V, INF);
        dist[src] = 0;

        // Релаксація ребер V-1 разів
        for (int i = 0; i < V - 1; i++) {
            for (int u = 0; u < V; u++) {
                for (const Edge& e : adj[u]) {
                    int v = e.dest;
                    double weight = e.weight;

                    if (dist[u] != INF && dist[u] + weight < dist[v]) {
                        dist[v] = dist[u] + weight;
                    }
                }
            }
        }

        // Перевіряємо наявність циклів з від'ємною вагою
        for (int u = 0; u < V; u++) {
            for (const Edge& e : adj[u]) {
                int v = e.dest;
                double weight = e.weight;

                if (dist[u] != INF && dist[u] + weight < dist[v]) {
                    return false;  // Знайдено цикл з від'ємною вагою
                }
            }
        }

        return true;
    }

    // Алгоритм Дейкстри з пірамідою Фібоначчі
    void dijkstraWithFibHeap(int src, std::vector<double>& dist) { //dist parameter?
        // Ініціалізуємо відстані
        dist.assign(V, INF);
        dist[src] = 0;

        FibonacciHeap heap;
        for (int v = 0; v < V; v++) {
            heap.insert(v, dist[v]);
        }

        while (!heap.isEmpty()) {
            auto [u, dist_u] = heap.extractMin();

            // Релаксація ребер
            for (const Edge& e : adj[u]) {
                int v = e.dest;
                double weight = e.weight;

                if (dist[u] != INF && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    heap.decreaseKey(v, dist[v]);
                }
            }
        }
    }

    // Алгоритм Джонсона
    std::vector<std::vector<double>> johnson() {
        // Створюємо новий граф з додатковою вершиною s
        Graph g(V + 1);
        for (int u = 0; u < V; u++) {
            for (const Edge& e : adj[u]) {
                g.addEdge(u, e.dest, e.weight);
            }
        }

        // Додаємо ребра з нульовою вагою від s до всіх вершин
        for (int v = 0; v < V; v++) {
            g.addEdge(V, v, 0);
        }

        // Запускаємо Беллмана-Форда з вершини s
        std::vector<double> h;
        if (!g.bellmanFord(V, h)) {
            std::cout << "Граф містить цикл з від'ємною вагою!" << std::endl;
            return std::vector<std::vector<double>>(V, std::vector<double>(V, INF));
        }

        // Перетворюємо ваги ребер
        for (int u = 0; u < V; u++) {
            for (Edge& e : adj[u]) {
                e.weight = e.weight + h[u] - h[e.dest];
            }
        }

        // Запускаємо Дейкстру з кожної вершини
        std::vector<std::vector<double>> dist(V, std::vector<double>(V, INF));

        for (int src = 0; src < V; src++) {
            dijkstraWithFibHeap(src, dist[src]);

            // Перетворюємо відстані назад
            for (int v = 0; v < V; v++) {
                if (dist[src][v] != INF) {
                    dist[src][v] = dist[src][v] - h[src] + h[v];
                }
            }
        }

        return dist;
    }
    void printMatrix(){
        std::vector<std::vector<double>> dist = johnson();
        std::cout << "Matrix of the shortest paths:" << std::endl;
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (dist[i][j] == INF)
                    std::cout << "INF\t";
                else
                    std::cout << dist[i][j] << "\t";
            }
            std::cout << std::endl;
        }
    }
};

int main() {
    int V;
    std::cout<<"enter number of vertices in graph"<<std::endl;
    std::cin>>V;
    Graph g(V);

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