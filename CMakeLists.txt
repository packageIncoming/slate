cmake_minimum_required(VERSION 3.20)

project(slate
  VERSION 0.0.0
  DESCRIPTION "An RV64GC interpreter"
  LANGUAGES CXX)

# ---------------------------------------------------------------------------
# Options
# ---------------------------------------------------------------------------
option(SLATE_ASAN "Build with the address and undefined-behavior sanitizers" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "" FORCE)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---------------------------------------------------------------------------
# Warnings and sanitizers, applied to every target through one interface target
# ---------------------------------------------------------------------------
# -Werror is not negotiable here. A warning in an interpreter core is usually a
# sign extension or a narrowing conversion, which is exactly the class of defect
# the conformance suite finds hardest to localize.
add_library(slate_flags INTERFACE)
target_compile_options(slate_flags INTERFACE -Wall -Wextra -Werror)

if(SLATE_ASAN)
  target_compile_options(slate_flags INTERFACE
    -fsanitize=address,undefined -fno-omit-frame-pointer -g)
  target_link_options(slate_flags INTERFACE -fsanitize=address,undefined)
  message(STATUS "slate: ASan and UBSan enabled")
endif()

# ---------------------------------------------------------------------------
# The image loader
# ---------------------------------------------------------------------------
add_library(slate_loader STATIC
  src/loader.cpp)
target_include_directories(slate_loader PUBLIC src)
target_link_libraries(slate_loader PUBLIC slate_flags)

# ---------------------------------------------------------------------------
# The command line front end
# ---------------------------------------------------------------------------
add_executable(slate
  src/main.cpp)
target_link_libraries(slate PRIVATE slate_loader slate_flags)

# ---------------------------------------------------------------------------
# Contract checks
# ---------------------------------------------------------------------------
# These are static_asserts. The target exists so that a resource handle which
# silently becomes copyable breaks the build instead of leaking.
add_executable(slate_static_asserts
  tests/static_asserts.cpp)
target_link_libraries(slate_static_asserts PRIVATE slate_loader slate_flags)

enable_testing()
add_test(NAME static_asserts COMMAND slate_static_asserts)
