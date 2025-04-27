#include "LFUCache.h"
#include <iostream>

LFUCache::LFUCache(int capacity) : capacity(capacity) {}

int LFUCache::get(int key) {
    std::lock_guard<std::mutex> lock(cache_mutex);
    if (cache.find(key) == cache.end()) return -1;

    touch(key);
    return cache[key].first;
}

void LFUCache::put(int key, int value) {
    std::lock_guard<std::mutex> lock(cache_mutex);
    
    if (capacity == 0) return;

    if (cache.find(key) != cache.end()) {
        cache[key].first = value;
        touch(key);
    } else {
        if (cache.size() >= capacity) {
            evict();
        }
        cache[key] = {value, 1};
        freq_map[1].push_front(key);
    }
}

void LFUCache::touch(int key) {
    int old_freq = cache[key].second;
    cache[key].second++;
    freq_map[old_freq].remove(key);
    freq_map[cache[key].second].push_front(key);
}

void LFUCache::evict() {
    for (auto& item : freq_map) {
        if (!item.second.empty()) {
            int key_to_evict = item.second.back();
            item.second.pop_back();
            cache.erase(key_to_evict);
            break;
        }
    }
}
