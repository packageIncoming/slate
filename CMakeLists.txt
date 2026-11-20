cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(slate LANGUAGES CXX)
add_compile_options(-Wall -Wextra -Werror)

option(SLATE_ASAN "Enable AddressSanitizer" OFF)
add_executable(slate src/main.cpp src/hart.cpp src/loader.cpp src/decoder.cpp src/memory.cpp src/elf.cpp)


# Apply ASAN if requested as option (DSLATE_ASAN=ON)
if (SLATE_ASAN)
    message(STATUS "SLATE_ASAN is on")
    target_compile_options(slate PRIVATE -fsanitize=address)
    target_link_options(slate PRIVATE -fsanitize=address)
endif()

