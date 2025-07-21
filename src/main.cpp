#include "CacheManager.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <vector>
#include <random>

// --- New Workload Simulations ---

// Workload where recent items are accessed frequently (good for LRU)
void simulateRecencyWorkload(CacheManager& manager, int items, int accesses) {
    std::cout << "\n>>> Starting Recency-Biased Workload (LRU should be better) <<<\n" << std::endl;
    for (int i = 0; i < accesses; ++i) {
        int key = i % items;
        manager.put(key, key * 10);
        manager.get(key);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

// Workload where a few items are accessed very frequently (good for LFU)
void simulateFrequencyWorkload(CacheManager& manager, int capacity, int accesses) {
    std::cout << "\n>>> Starting Frequency-Biased Workload (LFU should be better) <<<\n" << std::endl;
    std::random_device rd;
    std::mt19937 gen(rd());
    // Popular keys are in the range [0, capacity/2)
    // Unpopular keys are in the range [capacity, capacity*5)
    std::uniform_int_distribution<> popular_dist(0, capacity / 2);
    std::uniform_int_distribution<> unpopular_dist(capacity, capacity * 5);
    
    for (int i = 0; i < accesses; ++i) {
        int key;
        // 80% of accesses go to popular keys
        if (i % 5 < 4) { 
            key = popular_dist(gen);
        } else {
            key = unpopular_dist(gen);
        }
        manager.put(key, key * 10);
        manager.get(key);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <cache_capacity>" << std::endl;
        return 1;
    }
    size_t capacity = std::stoull(argv[1]);

    CacheManager manager(capacity);
    std::atomic<bool> stop_flag(false);

    std::thread policyManagerThread(&CacheManager::switchPolicy, &manager, std::ref(stop_flag));

    // Run workload that favors LRU
    simulateRecencyWorkload(manager, capacity + 5, 200);
    
    // Give time for policy check
    std::this_thread::sleep_for(std::chrono::seconds(6));

    // Run workload that favors LFU
    simulateFrequencyWorkload(manager, capacity, 200);

    // Let it run a bit longer to see the final switch
    std::this_thread::sleep_for(std::chrono::seconds(6));

    stop_flag.store(true);
    policyManagerThread.join();

    std::cout << "Program finished successfully." << std::endl;
    return 0;
}
