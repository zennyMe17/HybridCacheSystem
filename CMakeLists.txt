cmake_minimum_required(VERSION 3.10)
project(HybridCacheSystem)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")
add_executable(HybridCacheSystem ${SOURCES})

# --- Unit Tests ---
enable_testing()
add_executable(test_lru tests/test_lru.cpp src/LRUCache.cpp)
target_include_directories(test_lru PRIVATE include)
add_test(NAME LRUCacheTest COMMAND test_lru)