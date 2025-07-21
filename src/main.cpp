#include "CacheManager.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <vector>

void separator() {
    std::cout << "\n=======================================================\n" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <cache_capacity>" << std::endl;
        return 1;
    }
    size_t capacity = std::stoull(argv[1]);
    if (capacity < 5) {
        std::cerr << "Please use a capacity of 5 or greater for this simulation." << std::endl;
        return 1;
    }

    CacheManager manager(capacity);
    std::atomic<bool> stop_flag(false);
    std::thread policyManagerThread(&CacheManager::switchPolicy, &manager, std::ref(stop_flag));

    // --- PHASE 1: PROVING LFU IS BETTER ---
    separator();
    std::cout << ">>> PHASE 1: Proving LFU is better <<<" << std::endl;
    std::cout << "Workload: Accessing a few popular items repeatedly, then flooding with unique items." << std::endl;
    separator();

    // Step 1: Make keys 0, 1, 2 "popular" by accessing them frequently.
    for (int i = 0; i < 10; ++i) {
        manager.put(0, 0); manager.get(0);
        manager.put(1, 10); manager.get(1);
        manager.put(2, 20); manager.get(2);
    }
    // Step 2: Flood the cache with new, unique items.
    // LRU will discard 0, 1, 2. LFU will keep them because their frequency is high.
    for (int i = 0; i < capacity; ++i) {
        manager.put(100 + i, 1000 + i * 10);
    }
    // Step 3: Verify. Try to access the popular items.
    std::cout << "Verifying... LFU should have kept the popular items, LRU should not have.\n";
    manager.get(0); // LFU: Hit, LRU: Miss
    manager.get(1); // LFU: Hit, LRU: Miss
    manager.get(2); // LFU: Hit, LRU: Miss
    
    std::this_thread::sleep_for(std::chrono::seconds(6));
    manager.reset();

    // --- PHASE 2: PROVING LRU IS BETTER ---
    separator();
    std::cout << ">>> PHASE 2: Proving LRU is better <<<" << std::endl;
    std::cout << "Workload: A 'burst' of popular items, followed by a long scan of new items." << std::endl;
    separator();

    // Step 1: Create a "burst" of access for some items to pollute LFU's frequency counts.
    for (int i = 0; i < 5; ++i) {
        for(int j = 0; j < capacity; ++j) {
            manager.put(j, j*10); manager.get(j);
        }
    }
    // Step 2: Simulate a long scan of NEW, unique data that will never be used again.
    // LRU will correctly discard the old items. LFU will stubbornly hold them due to high frequency.
    for (int i = 0; i < 2 * capacity; ++i) {
        manager.put(100 + i, 1000 + i * 10);
    }
    // Step 3: Verify. Try to access the MOST RECENT items from the scan.
    std::cout << "Verifying... LRU should have kept the most recent items, LFU should not have.\n";
    for (int i = 0; i < capacity; ++i) {
        manager.get(100 + capacity + i); // LRU: Hit, LFU: Miss
    }

    std::this_thread::sleep_for(std::chrono::seconds(6));

    // --- Shutdown ---
    separator();
    std::cout << "--- Simulation Complete ---" << std::endl;
    stop_flag.store(true);
    policyManagerThread.join();

    std::cout << "Program finished successfully." << std::endl;
    return 0;
}
