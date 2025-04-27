#include "CacheManager.h"
#include <iostream>
#include <thread>

int main() {
    CacheManager manager(5);

    // Start a thread to periodically switch cache policies
    std::thread policySwitcher(&CacheManager::switchPolicy, &manager);

    // Simulate cache operations
    for (int i = 0; i < 100; i++) {
        manager.accessCache(i, i * 10);
    }

    policySwitcher.join();
    return 0;
}
