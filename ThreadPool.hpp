#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <vector>

class ThreadPool
{
public:
    ThreadPool(size_t threads) : 
        stop(false) 
    {
        for (size_t i = 0; i < threads; ++i) {
            workers.emplace_back([i, this]() {
                while(true){
                    std::function<void(size_t ш)> task;
                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        this->condition.wait(lock,
                            [this] { return this->stop || !this->tasks.empty(); 
                        });
                    }
                    if (this->stop && this->tasks.empty()) {
                         return;
                    }
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                    task(i);
                }
            });
        }
    }


    void enqueue(const std::function<void(uint8_t id)>& func) {
        std::lock_guard<std::mutex> lock(queue_mutex);
        tasks.emplace([func](uint8_t id) {func(id);});
        condition.notify_one();
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread& worker : workers) {
            worker.join();
        } 
    }

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void(uint8_t)>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};

#endif