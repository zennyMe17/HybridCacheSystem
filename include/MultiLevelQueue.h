#ifndef MULTILEVELQUEUE_H
#define MULTILEVELQUEUE_H

#include <queue>
#include <vector>
#include <mutex>

template <typename T>
class MultiLevelQueue {
public:
    MultiLevelQueue(int levels);
    void enqueue(int priority, T item);
    bool dequeue(int priority, T& item);

private:
    std::vector<std::queue<T>> queues;
    int levels;
    std::mutex queue_mutex;
};

#endif // MULTILEVELQUEUE_H
