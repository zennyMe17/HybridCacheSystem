#include "LFUCache.h"
#include <iostream>

LFUCache::LFUCache(size_t capacity) : capacity(capacity), hits(0), accesses(0) {}

// --- New Methods ---
double LFUCache::getHitRate() const {
    if (accesses == 0) return 0.0;
    return static_cast<double>(hits.load()) / accesses.load();
}

void LFUCache::resetStats() {
    hits = 0;
    accesses = 0;
}
// ---

int LFUCache::get(int key) {
    std::lock_guard<std::mutex> lock(cache_mutex);
    accesses++; // Track every access

    if (cache.find(key) == cache.end()) {
        return -1; // Miss
    }
    
    hits++; // Track hit
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

// Unchanged private helpers...
void LFUCache::touch(int key) {
    int old_freq = cache[key].second;
    cache[key].second++;
    freq_map[old_freq].remove(key);
    if (freq_map[old_freq].empty()) {
        freq_map.erase(old_freq);
    }
    freq_map[cache[key].second].push_front(key);
}

void LFUCache::evict() {
    int min_freq = -1;
    for(auto const& [freq, keys] : freq_map) {
        if(min_freq == -1 || freq < min_freq) {
            min_freq = freq;
        }
    }
    if(min_freq != -1) {
        int key_to_evict = freq_map[min_freq].back();
        freq_map[min_freq].pop_back();
        if (freq_map[min_freq].empty()) {
            freq_map.erase(min_freq);
        }
        cache.erase(key_to_evict);
    }
}

// Add this function to LFUCache.cpp
void LFUCache::reset() {
    std::lock_guard<std::mutex> lock(cache_mutex);
    cache.clear();
    freq_map.clear();
    resetStats();
}
