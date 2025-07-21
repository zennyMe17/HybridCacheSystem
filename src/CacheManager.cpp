#include "CacheManager.h"
#include <iostream>
#include <thread>

CacheManager::CacheManager(int capacity)
    : lfu_cache(capacity), lru_cache(capacity), current_policy(LRU) {}

void CacheManager::setPolicy(int policy) {
    std::lock_guard<std::mutex> lock(policy_mutex);
    current_policy = policy;
}

void CacheManager::accessCache(int key, int value) {
    std::lock_guard<std::mutex> lock(access_mutex);
    if (current_policy == LFU) {
        lfu_cache.put(key, value);
    } else if (current_policy == LRU) {
        lru_cache.put(key, value);
    }
}

// Modified to use the stop flag
void CacheManager::switchPolicy(std::atomic<bool>& stop_flag) {
    // Loop only until the flag is set to true
    while (!stop_flag.load()) {
        std::this_thread::sleep_for(std::chrono::seconds(2)); // Shortened for test
        
        // Check the flag again in case it was set while sleeping
        if (stop_flag.load()) break;

        setPolicy(current_policy == LFU ? LRU : LFU);
        std::cout << "Cache policy switched!" << std::endl;
    }
}
