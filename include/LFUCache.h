#ifndef LFUCACHE_H
#define LFUCACHE_H

#include <unordered_map>
#include <list>
#include <mutex>

class LFUCache {
public:
    LFUCache(int capacity);
    int get(int key);
    void put(int key, int value);

private:
    void touch(int key);
    void evict();

    int capacity;
    std::unordered_map<int, std::pair<int, int>> cache; // key -> (value, frequency)
    std::unordered_map<int, std::list<int>> freq_map;   // frequency -> list of keys
    std::mutex cache_mutex;
};

#endif // LFUCACHE_H
