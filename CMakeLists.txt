cmake_minimum_required(VERSION 3.10)

project(shm LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
