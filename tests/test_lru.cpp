#include "../include/LRUCache.h"
#include <cassert>
#include <iostream>

int main() {
    LRUCache cache(2);

    cache.put(1, 100);
    cache.put(2, 200);
    assert(cache.get(1) == 100); // Should return 100

    cache.put(3, 300); // This should evict key 2
    assert(cache.get(2) == -1);  // Should return -1 (not found)

    cache.put(4, 400); // This should evict key 1
    assert(cache.get(1) == -1);  // Should return -1 (not found)
    assert(cache.get(3) == 300); // Should return 300
    assert(cache.get(4) == 400); // Should return 400

    std::cout << "All LRUCache tests passed!\n";
    return 0;
}