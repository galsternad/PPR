#pragma once

#include "sync_queue.hpp"
#include <thread>

template <typename T>
class Thread {
    private:
        virtual void run(SyncQueue<T>& tasks) = 0;
        Thread(const Thread& thread);
    
    protected:
        std::thread thread;

    public:
        Thread() : thread() { }
        
        virtual ~Thread() {
            if(!thread.joinable()) {
                return;
            }
            
            this->thread.join();
        }
};