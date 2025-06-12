#pragma once
#include <queue>
#include <condition_variable>
#include <functional>
#include <thread>
#include <future>

// Thread Pool Pattern
///@brief the class for working with threads
class ThreadPool {
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;

public:
    ThreadPool(size_t threads);

    ///@brief the static method for gettinf an optimal number of threads
    static size_t getOptimalThreadCount() {
        return std::thread::hardware_concurrency();
    }

    // Метод для отримання поточної кількості потоків
    size_t getThreadCount() const {
        return workers.size();
    }
    ///@brief the main function for managing the threads
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<std::invoke_result_t<F, Args...>> {
        using return_type = std::invoke_result_t<F, Args...>;

        auto task = std::make_shared<std::packaged_task<return_type()>>(
                std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            if(stop)
                throw std::runtime_error("enqueue on stopped ThreadPool");
            tasks.emplace([task](){ (*task)(); });
        }
        condition.notify_one();
        return res;
    }

    ~ThreadPool();
};