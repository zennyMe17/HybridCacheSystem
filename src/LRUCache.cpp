#include "LRUCache.h"
#include <algorithm>

LRUCache::LRUCache(size_t capacity) : capacity(capacity), hits(0), accesses(0) {}

// --- New Methods ---
double LRUCache::getHitRate() const {
    if (accesses == 0) return 0.0;
    return static_cast<double>(hits.load()) / accesses.load();
}

void LRUCache::resetStats() {
    hits = 0;
    accesses = 0;
}
// ---

int LRUCache::get(int key) {
    std::lock_guard<std::mutex> lock(cache_mutex);
    accesses++; // Track every access

    if (cache.find(key) == cache.end()) {
        return -1; // Miss
    }

    hits++; // Track hit
    moveToFront(key);
    return cache[key].first;
}

void LRUCache::put(int key, int value) {
    std::lock_guard<std::mutex> lock(cache_mutex);
    if (capacity == 0) return;

    if (cache.find(key) != cache.end()) {
        cache[key].first = value;
        moveToFront(key);
    } else {
        if (cache.size() >= capacity) {
            evict();
        }
        cache[key] = {value, 0};
        access_order.push_front(key);
    }
}

// Unchanged private helpers...
void LRUCache::moveToFront(int key) {
    access_order.remove(key);
    access_order.push_front(key);
}

void LRUCache::evict() {
    if (access_order.empty()) return;
    int key_to_evict = access_order.back();
    access_order.pop_back();
    cache.erase(key_to_evict);
}

// Add this function to LRUCache.cpp
void LRUCache::reset() {
    std::lock_guard<std::mutex> lock(cache_mutex);
    cache.clear();
    access_order.clear();
    resetStats();
}
