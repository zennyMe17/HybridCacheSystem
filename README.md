# HybridCacheSystem

HybridCacheSystem is a modular and efficient caching framework written in modern C++. It provides a ready-to-use foundation for building high-performance, multi-layered cache systems, featuring classic algorithms like LRU (Least Recently Used) and LFU (Least Frequently Used) as well as an extensible multi-level queue for advanced scenarios. The core CacheManager component orchestrates these policies, making it easy to combine and customize cache behavior for complex applications.

---

## 📂 Project Structure

```
include/
│   CacheManager.h       # Interface for a unified cache manager
│   LFUCache.h           # Header for LFU (Least Frequently Used) cache
│   LRUCache.h           # Header for LRU (Least Recently Used) cache
│   MultiLevelQueue.h    # Header for multi-level queue, supporting hybrid/multi-tier caching
src/
│   CacheManager.cpp     # Implementation of the cache manager logic
│   LFUCache.cpp         # Implementation of LFU cache
│   LRUCache.cpp         # Implementation of LRU cache
│   MultiLevelQueue.cpp  # Implementation of multi-level queue
│   main.cpp             # Entry point with interactive demo
```

---

## 🚀 Getting Started

### 1. Prerequisites

- C++17 compatible compiler (e.g., g++ 7+, clang 6+, MSVC 2017+)
- CMake 3.10 or newer (for recommended build)
- Standard C++ STL (no external dependencies)

### 2. Build Instructions

```bash
git clone https://github.com/zennyMe17/HybridCacheSystem.git
cd HybridCacheSystem
mkdir build && cd build
cmake ..
make
```

This will generate an executable (typically HybridCacheSystem or main) in the build directory.

---

## 🛠️ How It Works

HybridCacheSystem allows you to:

- Use LRU or LFU as standalone caches for fast memory-based key-value storage.
- Combine multiple cache strategies using the MultiLevelQueue for advanced, tiered caching (e.g., hot/cold data separation).
- Centrally manage all cache operations (set, get, evict, stats) using CacheManager, which abstracts away the underlying policy.

### Main Components

#### 1. LRUCache
- Evicts the least recently accessed items first.
- Good for workloads where recent access is a strong predictor of future use.

#### 2. LFUCache
- Evicts the least frequently accessed items.
- Ideal for workloads with frequently repeating keys.

#### 3. MultiLevelQueue
- Supports multi-tier caching (for example, combining RAM and SSD, or different cache policies).
- Can be configured to cascade evictions across levels.

#### 4. CacheManager
- Unified entry point for cache operations.
- Can be configured to use any combination of the above caches.
- Handles cache lookup, insertion, eviction, and statistics.

---

## 🕹️ Interactive Demo

Run the demo executable:

```bash
./HybridCacheSystem
```

Example interactive session:

```
> set foo 123
OK
> get foo
123
> set bar 456
OK
> stats
{"LRU_items": 1, "LFU_items": 1, ...}
> del foo
OK
> get foo
NOT FOUND
> exit
```

---

## 🧑‍💻 Example Usage in C++

```cpp
#include "CacheManager.h"

int main() {
    // Create a cache manager with a total size of 100 items
    CacheManager manager(100);

    // Add some items
    manager.set("alpha", 10);
    manager.set("beta", 20);

    // Retrieve items
    int value = manager.get("alpha"); // returns 10

    // Remove an item
    manager.del("beta");

    // Print statistics
    manager.stats();
}
```

---

## ⚙️ Customization & Extension

- To change cache policy, instantiate `CacheManager` with different cache objects (see code in `main.cpp`).
- To add a new policy, create a new cache class (e.g., MRUCache for Most Recently Used), following the structure in `include/LRUCache.h` and `src/LRUCache.cpp`.
- Plug the new cache into CacheManager for hybrid strategies.

---

## 📚 Advanced Topics

- **Multi-level caching:** Use `MultiLevelQueue` to combine fast (RAM) and slow (disk) caches for maximum efficiency.
- **Eviction tuning:** Adjust cache sizes and policies to suit your application's data access patterns.
- **Thread safety:** Current implementation is single-threaded; for multi-threaded use, wrap cache operations with mutexes.

---

## 🧪 Testing

- The interactive demo (main.cpp) covers common use-cases and edge cases.
- For unit tests, create test cases for each cache class and simulate various access/eviction patterns.

---

## 🤝 Contributing

- Issues and pull requests are welcome!
- Please open an issue to discuss bugs or feature requests.
- Follow consistent C++ style and document your code.

---

## ❓ FAQ

**Q: What is the main use case for this project?**  
A: It's a learning resource and starting point for building high-performance caching systems in C++. Use it to power applications that need fast, layered key-value storage, or as a reference for implementing cache algorithms.

**Q: Can I use this in production?**  
A: This is a reference implementation. For production, ensure you review, test, and extend it as needed (e.g., add thread safety, persistence, advanced metrics).

**Q: How do I switch between LRU and LFU?**  
A: Change the cache objects passed to CacheManager. See `main.cpp` for examples.

---

## 🕹️ Quick Links

- Interactive Demo: `src/main.cpp`
- LRU Cache: `include/LRUCache.h`, `src/LRUCache.cpp`
- LFU Cache: `include/LFUCache.h`, `src/LFUCache.cpp`
- Multi-Level Queue: `include/MultiLevelQueue.h`, `src/MultiLevelQueue.cpp`
- Cache Manager: `include/CacheManager.h`, `src/CacheManager.cpp`

---

Happy caching and learning! 🚀
