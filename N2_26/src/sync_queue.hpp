#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

template <typename T>
class SyncQueue {
    private:
        std::queue<T> tasks;
        std::mutex mutex;
        std::condition_variable condition;
        bool isStopped;

    public:
        SyncQueue() :
            tasks(),
            mutex(),
            condition(),
            isStopped(false) { }

        bool push(const T& data);
        bool pop(T& data);
        void stop();
};

template <typename T>
bool SyncQueue<T>::push(const T& task) {
    std::unique_lock<std::mutex> lock(mutex);

    if(isStopped) {
        return false;
    }

    tasks.push(task);

    condition.notify_one();

    return true;
}

template <typename T>
bool SyncQueue<T>::pop(T& task) {
    std::unique_lock<std::mutex> lock(mutex);

    while(tasks.empty() && !isStopped) {
        condition.wait(lock);
    }

    if(tasks.empty() && isStopped) {
        return false;
    }

    task = tasks.front();
    tasks.pop();

    return true;
}

template <typename T>
void SyncQueue<T>::stop() {
    std::lock_guard<std::mutex> lock(mutex);

    isStopped = true;

    condition.notify_all();
}