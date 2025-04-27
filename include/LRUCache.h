#ifndef LRUCACHE_H
#define LRUCACHE_H

#include <unordered_map>
#include <list>
#include <mutex>

class LRUCache {
public:
    LRUCache(int capacity);
    int get(int key);
    void put(int key, int value);

private:
    void moveToFront(int key);
    void evict();

    int capacity;
    std::unordered_map<int, std::pair<int, int>> cache; // key -> (value)
    std::list<int> access_order;                         // order of keys (most recent to least)
    std::mutex cache_mutex;
};

#endif // LRUCACHE_H
