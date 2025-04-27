#include "MultiLevelQueue.h"

template <typename T>
MultiLevelQueue<T>::MultiLevelQueue(int levels) : levels(levels) {
    queues.resize(levels);
}

template <typename T>
void MultiLevelQueue<T>::enqueue(int priority, T item) {
    std::lock_guard<std::mutex> lock(queue_mutex);
    if (priority < 0 || priority >= levels) return;
    queues[priority].push(item);
}

template <typename T>
bool MultiLevelQueue<T>::dequeue(int priority, T& item) {
    std::lock_guard<std::mutex> lock(queue_mutex);
    if (priority < 0 || priority >= levels || queues[priority].empty()) return false;

    item = queues[priority].front();
    queues[priority].pop();
    return true;
}
