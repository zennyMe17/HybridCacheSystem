#ifndef LRUCACHE_H
#define LRUCACHE_H

#include <unordered_map>
#include <list>
#include <mutex>
#include <atomic>

class LRUCache {
public:
    LRUCache(size_t capacity);
    int get(int key);
    void put(int key, int value);

    // --- New Methods for Stats ---
    double getHitRate() const;
    void resetStats();

private:
    void moveToFront(int key);
    void evict();

    size_t capacity;
    std::unordered_map<int, std::pair<int, int>> cache;
    std::list<int> access_order;
    std::mutex cache_mutex;

    // --- New Stats Counters ---
    std::atomic<size_t> hits;
    std::atomic<size_t> accesses;
};

#endif // LRUCACHE_H
