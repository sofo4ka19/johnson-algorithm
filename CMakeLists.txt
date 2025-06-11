cmake_minimum_required(VERSION 3.16)
project(JohnsonAlgorithm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

find_package(Threads REQUIRED)

# Налаштування GoogleTest
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

# Основні джерела
set(SOURCES
        src/fibonacci_heap.cpp
        src/graph.cpp
        src/thread_pool.cpp
)

# Головний виконуваний файл
add_executable(johnson_main src/main.cpp ${SOURCES})
target_link_libraries(johnson_main PRIVATE Threads::Threads)
target_include_directories(johnson_main PRIVATE include)

# Тести
add_executable(tests
        tests/test_main.cpp
        tests/test_fibonacci_heap.cpp
        tests/test_graph.cpp
        ${SOURCES}
)
target_include_directories(tests PRIVATE
        include
        ${googletest_SOURCE_DIR}/include  # Критично важливо!
)
target_link_libraries(tests PRIVATE
        gtest
        gtest_main
        Threads::Threads
)

enable_testing()
add_test(NAME unit_tests COMMAND tests)