#pragma once

#include "thread.hpp"
#include "sync_queue.hpp"

template <typename T>
class Reactor : public Thread<T> {
    public:
        Reactor(SyncQueue<T>& tasks) {
            this->thread = std::thread([this, &tasks]() {
                this->run(tasks);
            });
        }

        virtual void run(SyncQueue<T>& tasks) override;
};