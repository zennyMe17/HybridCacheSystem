#include "CacheManager.h"
#include <iostream>
#include <thread>
#include <iomanip> // For std::fixed and std::setprecision

CacheManager::CacheManager(size_t capacity)
    : lfu_cache(capacity), lru_cache(capacity), current_policy(LRU_P) {}

// NEW: Send GET traffic to both caches to compare performance
int CacheManager::get(int key) {
    // Both caches process the get request to update their internal state and stats
    int lru_val = lru_cache.get(key);
    int lfu_val = lfu_cache.get(key);

    // Return the value from the currently active policy
    if (current_policy.load() == LRU_P) {
        return lru_val;
    } else {
        return lfu_val;
    }
}

// NEW: Send PUT traffic to both caches
void CacheManager::put(int key, int value) {
    lru_cache.put(key, value);
    lfu_cache.put(key, value);
}

std::string CacheManager::getCurrentPolicyName() const {
    return (current_policy.load() == LRU_P) ? "LRU" : "LFU";
}

// REWRITTEN: The adaptive switching logic
void CacheManager::switchPolicy(std::atomic<bool>& stop_flag) {
    while (!stop_flag.load()) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        if (stop_flag.load()) break;

        std::lock_guard<std::mutex> lock(policy_mutex);
        
        double lru_hit_rate = lru_cache.getHitRate();
        double lfu_hit_rate = lfu_cache.getHitRate();

        std::cout << "\n--- Policy Check ---" << std::endl;
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Current Policy: " << getCurrentPolicyName() << std::endl;
        std::cout << "LRU Hit Rate: " << lru_hit_rate * 100 << "%" << std::endl;
        std::cout << "LFU Hit Rate: " << lfu_hit_rate * 100 << "%" << std::endl;

        // Adaptive switching condition
        // Switch if the other policy is performing better by a meaningful margin (e.g., 5%)
        if (current_policy == LRU_P && lfu_hit_rate > lru_hit_rate + 0.05) {
            current_policy = LFU_P;
            std::cout << "Switching to LFU policy due to better performance." << std::endl;
            // Reset stats after switching to start a new measurement period
            lru_cache.resetStats();
            lfu_cache.resetStats();
        } else if (current_policy == LFU_P && lru_hit_rate > lfu_hit_rate + 0.05) {
            current_policy = LRU_P;
            std::cout << "Switching to LRU policy due to better performance." << std::endl;
            lru_cache.resetStats();
            lfu_cache.resetStats();
        } else {
            std::cout << "No policy change needed." << std::endl;
        }
        std::cout << "--------------------\n" << std::endl;
    }
}
