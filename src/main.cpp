#include "CacheManager.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <string> // Required for std::stoi

int main(int argc, char* argv[]) {
    // --- 1. Read Cache Capacity from Command Line ---
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <cache_capacity>" << std::endl;
        return 1; // Exit with an error
    }

    int capacity = 0;
    try {
        capacity = std::stoi(argv[1]);
    } catch (const std::exception& e) {
        std::cerr << "Error: Invalid number for cache capacity." << std::endl;
        return 1;
    }

    if (capacity <= 0) {
        std::cerr << "Error: Cache capacity must be a positive number." << std::endl;
        return 1;
    }

    std::cout << "Initializing cache with capacity: " << capacity << std::endl;
    CacheManager manager(capacity); // Use the capacity from user input
    std::atomic<bool> stop_flag(false);

    // --- 2. The Rest of the Program Runs as Before ---
    std::thread policySwitcher(&CacheManager::switchPolicy, &manager, std::ref(stop_flag));

    std::cout << "Simulating cache access..." << std::endl;
    for (int i = 0; i < (capacity * 4); ++i) { // Simulate based on capacity
        manager.accessCache(i, i * 10);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "Running for 5 seconds before shutdown..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));

    std::cout << "Signaling policy switcher to stop..." << std::endl;
    stop_flag.store(true);
    policySwitcher.join();

    std::cout << "Program finished successfully." << std::endl;
    return 0;
}
