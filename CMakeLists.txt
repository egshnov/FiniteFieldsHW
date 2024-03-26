cmake_minimum_required(VERSION 3.10)
project(FiniteFieldsHW C)

set(CMAKE_C_STANDARD 11)

enable_testing()

add_subdirectory(lib)
add_subdirectory(tests)