#ifndef CACHEMANAGER_H
#define CACHEMANAGER_H

#include "LFUCache.h"
#include "LRUCache.h"
#include <atomic>
#include <mutex>
#include <string>

class CacheManager {
public:
    CacheManager(size_t capacity);
    int get(int key);
    void put(int key, int value);
    void switchPolicy(std::atomic<bool>& stop_flag);
    std::string getCurrentPolicyName() const;
    void reset();

private:
    enum Policy { LFU_P, LRU_P };

    LFUCache lfu_cache;
    LRUCache lru_cache;
    std::atomic<Policy> current_policy;
    std::mutex policy_mutex;
};

#endif // CACHEMANAGER_H
