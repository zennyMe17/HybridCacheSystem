#include "CacheManager.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

int main() {
    CacheManager manager(5);
    std::atomic<bool> stop_flag(false); // The stop flag

    // Pass a reference to the flag to the thread
    std::thread policySwitcher(&CacheManager::switchPolicy, &manager, std::ref(stop_flag));

    // Simulate cache operations
    std::cout << "Simulating cache access..." << std::endl;
    for (int i = 0; i < 20; i++) {
        manager.accessCache(i, i * 10);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Let the policy switcher run a few times
    std::cout << "Running for 5 seconds before shutdown..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // --- Graceful Shutdown ---
    std::cout << "Signaling policy switcher to stop..." << std::endl;
    stop_flag.store(true); // Set the flag to true
    policySwitcher.join(); // Wait for the thread to finish its loop

    std::cout << "Program finished successfully." << std::endl;
    return 0;
}
