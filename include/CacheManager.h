#ifndef CACHEMANAGER_H
#define CACHEMANAGER_H

#include "LFUCache.h"
#include "LRUCache.h"
#include <atomic>
#include <mutex>

class CacheManager {
public:
    CacheManager(int capacity);
    void setPolicy(int policy);
    void accessCache(int key, int value);
    void switchPolicy();

private:
    enum { LFU = 0, LRU = 1 };

    LFUCache lfu_cache;
    LRUCache lru_cache;
    std::atomic<int> current_policy;  // Use atomic for thread safety
    std::mutex policy_mutex;
    std::mutex access_mutex;
};

#endif // CACHEMANAGER_H
