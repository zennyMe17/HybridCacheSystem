#include "LRUCache.h"
#include <algorithm> // Required for std::find

LRUCache::LRUCache(int capacity) : capacity(capacity) {}

// Private helper to move a key to the front of the access_order list
void LRUCache::moveToFront(int key) {
    access_order.remove(key);
    access_order.push_front(key);
}

// Private helper to evict the least recently used item
void LRUCache::evict() {
    if (access_order.empty()) return;
    
    // Get the least recently used key from the back of the list
    int key_to_evict = access_order.back();
    access_order.pop_back();
    
    // Erase it from the cache map
    cache.erase(key_to_evict);
}

int LRUCache::get(int key) {
    std::lock_guard<std::mutex> lock(cache_mutex);

    // If key is not in the cache, return -1
    if (cache.find(key) == cache.end()) {
        return -1;
    }

    // Key exists, so it's being used. Move it to the front.
    moveToFront(key);
    
    // Note: Assuming the value is stored in the 'first' element of the pair
    return cache[key].first;
}

void LRUCache::put(int key, int value) {
    std::lock_guard<std::mutex> lock(cache_mutex);
    
    if (capacity == 0) return;

    // If key already exists, update its value and move it to the front
    if (cache.find(key) != cache.end()) {
        cache[key].first = value;
        moveToFront(key);
    } else {
        // If the cache is full, evict the least recently used item first
        if (cache.size() >= capacity) {
            evict();
        }
        // Add the new key-value pair
        cache[key] = {value, 0}; // Storing value in .first, .second is unused per your header
        access_order.push_front(key);
    }
}
