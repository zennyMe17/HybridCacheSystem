# HybridCacheSystem

HybridCacheSystem is a modular and efficient caching framework written in modern C++. It provides a ready-to-use foundation for building high-performance, multi-layered cache systems, featuring classic algorithms like LRU (Least Recently Used) and LFU (Least Frequently Used) as well as an extensible multi-level queue for advanced scenarios. The core CacheManager component orchestrates these policies, making it easy to combine and customize cache behavior for complex applications.

---

## Project Structure

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
tests/
│   test_lru.cpp         # Example unit test for LRUCache
.github/
│   workflows/
│       ci.yml           # GitHub Actions workflow for CI/CD
```

---

## Getting Started

### Prerequisites

- C++17 compatible compiler (e.g., g++ 7+, clang 6+, MSVC 2017+)
- CMake 3.10 or newer (for recommended build)
- Standard C++ STL (no external dependencies)

### Build Instructions

```bash
git clone https://github.com/zennyMe17/HybridCacheSystem.git
cd HybridCacheSystem
mkdir build && cd build
cmake ..
make
```

This will generate an executable (typically HybridCacheSystem or main) in the build directory.

---

## Automation & Continuous Integration

This project uses GitHub Actions for automatic building and testing.  
Every push and pull request to `main` or `master` will trigger an automated build and run the unit tests.

- The workflow checks out your code, installs dependencies, configures with CMake, builds, and runs all unit tests with CTest.
- To add more tests, simply add `.cpp` test files in the `tests/` directory and update `CMakeLists.txt` accordingly.

**Workflow badge:**

```markdown
![CI](https://github.com/zennyMe17/HybridCacheSystem/actions/workflows/ci.yml/badge.svg)
```

You can find the workflow file at `.github/workflows/ci.yml`.

---

## How It Works

HybridCacheSystem allows you to:

- Use LRU or LFU as standalone caches for fast memory-based key-value storage.
- Combine multiple cache strategies using the MultiLevelQueue for advanced, tiered caching (e.g., hot/cold data separation).
- Centrally manage all cache operations (set, get, evict, stats) using CacheManager, which abstracts away the underlying policy.

### Main Components

**LRUCache**  
Evicts the least recently accessed items first. Useful for workloads where recent access is a strong predictor of future use.

**LFUCache**  
Evicts the least frequently accessed items. Ideal for workloads with frequently repeating keys.

**MultiLevelQueue**  
Supports multi-tier caching (e.g., combining RAM and SSD, or different cache policies). Can be configured to cascade evictions across levels.

**CacheManager**  
Unified entry point for cache operations. Can be configured to use any combination of the above caches. Handles cache lookup, insertion, eviction, and statistics.

---

## Advanced Topics

- **Multi-level caching:** Use `MultiLevelQueue` to combine fast (RAM) and slow (disk) caches for maximum efficiency.
- **Eviction tuning:** Adjust cache sizes and policies to suit your application's data access patterns.
- **Thread safety:** Current implementation is single-threaded; for multi-threaded use, wrap cache operations with mutexes.

---

## Contributing

- Issues and pull requests are welcome!
- Please open an issue to discuss bugs or feature requests.
- Follow consistent C++ style and document your code.

---

Happy caching and learning!
