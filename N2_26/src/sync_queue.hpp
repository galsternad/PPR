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
            mutex(std::mutex()),
            condition(std::condition_variable()),
            isStopped(false) { }

        bool push(const T& data);
        bool pop(T& data);
        void stop();
};