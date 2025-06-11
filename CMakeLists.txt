cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)

# Пошук усіх .cpp в src/
file(GLOB SOURCES "src/*.cpp")

# Папка з .h файлами
include_directories(include)

add_executable(MyProject ${SOURCES})
