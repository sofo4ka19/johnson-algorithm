cmake_minimum_required(VERSION 3.16)
project(JohnsonAlgorithm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Важливо для CLion - експортуємо команди компіляції
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Threads REQUIRED)

# Налаштування GoogleTest
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/v1.14.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
)

# Встановлюємо опції перед завантаженням
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(BUILD_GTEST ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# Отримуємо шляхи до GoogleTest після завантаження
FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
endif()

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

# Тести з детальним налаштуванням include директорій
add_executable(tests
        tests/test_main.cpp
        tests/test_fibonacci_heap.cpp
        tests/test_graph.cpp
        ${SOURCES}
)

# КРИТИЧНО ВАЖЛИВО: додаємо всі необхідні шляхи для CLion
target_include_directories(tests PRIVATE
        include
        ${googletest_SOURCE_DIR}/googletest/include
        ${googletest_SOURCE_DIR}/googletest
        ${googletest_SOURCE_DIR}/googlemock/include
        ${googletest_SOURCE_DIR}/googlemock
)

# Зв'язування бібліотек
target_link_libraries(tests PRIVATE
        gtest
        gtest_main
        Threads::Threads
)

# Додаткові властивості для IDE
set_target_properties(tests PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)

# Додаємо визначення для кращої сумісності
target_compile_definitions(tests PRIVATE
        GTEST_HAS_PTHREAD=1
)

enable_testing()
add_test(NAME unit_tests COMMAND tests)

# Додаткове налаштування для CLion
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(tests PRIVATE -g -O0)
endif()

# ---------- DOXYGEN ---------- #
find_package(Doxygen REQUIRED)

set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

add_custom_target(doc_doxygen
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
)