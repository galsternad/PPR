#include "reactor.hpp"
#include "sync_queue.hpp"

template <typename T>
void Reactor<T>::run(SyncQueue<T>& tasks) {
    T task = T();

    while(tasks.pop(task)) {
        task();
    }
}