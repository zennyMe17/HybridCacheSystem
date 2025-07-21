#include "CacheManager.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <vector>
#include <random>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <cache_capacity>" << std::endl;
        return 1;
    }
    size_t capacity = std::stoull(argv[1]);
    if (capacity == 0) {
        std::cerr << "Capacity must be greater than 0." << std::endl;
        return 1;
    }

    CacheManager manager(capacity);
    std::atomic<bool> stop_flag(false);

    // Start the background thread that will check and switch policies
    std::thread policyManagerThread(&CacheManager::switchPolicy, &manager, std::ref(stop_flag));

    std::cout << "--- Starting Simulation ---" << std::endl;
    std::cout << "Initial Policy: " << manager.getCurrentPolicyName() << std::endl;
    std::cout << "---------------------------\n" << std::endl;
    
    // =================================================================
    // PHASE 1: LRU-biased Workload
    // Access a sequence of unique items. LRU excels because it remembers recent items.
    // LFU will struggle as every item has a frequency of 1.
    // =================================================================
    std::cout << ">>> PHASE 1: Running LRU-Biased Workload (Sequential Access)... <<<\n" << std::endl;
    // Load more items than the cache can hold to force evictions
    for (int i = 0; i < capacity + 5; ++i) {
        manager.put(i, i * 10);
    }
    // Now, access the MOST RECENT items. LRU should have kept them.
    for (int i = 5; i < capacity + 5; ++i) {
        manager.get(i);
    }

    // Wait for the policy manager to run and evaluate
    std::this_thread::sleep_for(std::chrono::seconds(6));


    // =================================================================
    // PHASE 2: LFU-biased Workload
    // Access a few "popular" items many times, and some "unpopular" items once.
    // LFU will keep the popular items. LRU will get tricked into evicting
    // them when an unpopular item is accessed more recently.
    // =================================================================
    std::cout << "\n>>> PHASE 2: Running LFU-Biased Workload (Popular Items Access)... <<<\n" << std::endl;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> unpopular_dist(capacity * 2, capacity * 10);

    // Access 3 "popular" keys 20 times each
    for (int i = 0; i < 20; ++i) {
        manager.get(0);
        manager.get(1);
        manager.get(2);
        // Occasionally access a new, "unpopular" item
        if (i % 4 == 0) {
            manager.get(unpopular_dist(gen));
        }
    }

    // Wait for the policy manager to run and evaluate again
    std::this_thread::sleep_for(std::chrono::seconds(6));
    
    // --- Shutdown ---
    std::cout << "\n--- Simulation Complete ---\n" << std::endl;
    stop_flag.store(true);
    policyManagerThread.join();

    std::cout << "Program finished successfully." << std::endl;
    return 0;
}
