#ifndef LFUCACHE_H
#define LFUCACHE_H

#include <unordered_map>
#include <list>
#include <mutex>
#include <atomic>

class LFUCache {
public:
    LFUCache(size_t capacity);
    int get(int key);
    void put(int key, int value);
    // Add this line in both LRUCache.h and LFUCache.h
    void reset();

    // --- New Methods for Stats ---
    double getHitRate() const;
    void resetStats();

private:
    void touch(int key);
    void evict();
    

    size_t capacity;
    std::unordered_map<int, std::pair<int, int>> cache; // key -> {value, frequency}
    std::unordered_map<int, std::list<int>> freq_map;  // frequency -> list of keys
    std::mutex cache_mutex;

    // --- New Stats Counters ---
    std::atomic<size_t> hits;
    std::atomic<size_t> accesses;
};

#endif // LFUCACHE_H
