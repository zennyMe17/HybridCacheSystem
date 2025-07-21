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
    if (capacity < 5) { // The simulation needs a capacity of at least 5 to work well
        std::cerr << "Please use a capacity of 5 or greater for this simulation." << std::endl;
        return 1;
    }

    CacheManager manager(capacity);
    std::atomic<bool> stop_flag(false);

    std::thread policyManagerThread(&CacheManager::switchPolicy, &manager, std::ref(stop_flag));

    separator();
    std::cout << ">>> PHASE 1: Proving LFU is better <<<" << std::endl;
    std::cout << "Workload: Accessing a few popular items repeatedly." << std::endl;
    separator();
    
    // Step 1: Access popular items (0, 1, 2) multiple times to increase their frequency
    for(int i = 0; i < 10; ++i) {
        manager.put(0, 0); 
        manager.put(1, 10);
        manager.put(2, 20);
    }

    // Step 2: Access a series of unique, one-off items to flush out an LRU cache
    for(int i = 0; i < capacity; ++i) {
        manager.put(100 + i, 1000 + i*10);
    }
    
    // Step 3: Now, try to access the popular items again
    std::cout << "Verifying... trying to access popular items (0, 1, 2)." << std::endl;
    manager.get(0); // LFU should HIT, LRU should MISS
    manager.get(1); // LFU should HIT, LRU should MISS
    manager.get(2); // LFU should HIT, LRU should MISS
    
    // Wait for the policy manager to run and see the performance difference
    std::this_thread::sleep_for(std::chrono::seconds(6));
    
    // Reset for the next phase
    manager.reset();

    separator();
    std::cout << ">>> PHASE 2: Proving LRU is better <<<" << std::endl;
    std::cout << "Workload: A long, sequential scan of unique items." << std::endl;
    separator();
    
    // Step 1: Fill the cache with unique items
    for(int i = 0; i < capacity; ++i) {
        manager.put(i, i*10);
    }
    
    // Step 2: Access the most recent half of those items again
    std::cout << "Verifying... trying to access most recent items." << std::endl;
    for(int i = capacity / 2; i < capacity; ++i) {
        manager.get(i); // LRU should HIT, LFU performance will depend on its eviction choices
    }

    // Wait for the policy manager to run again
    std::this_thread::sleep_for(std::chrono::seconds(6));

    // --- Shutdown ---
    separator();
    std::cout << "--- Simulation Complete ---" << std::endl;
    stop_flag.store(true);
    policyManagerThread.join();

    std::cout << "Program finished successfully." << std::endl;
    return 0;
}
