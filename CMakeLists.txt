cmake_minimum_required(VERSION 3.20)

project(catacomb CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)     # hard error if compiler doesn't support it
set(CMAKE_CXX_EXTENSIONS OFF)           # use -std=c++20, not -std=gnu++20

# ── build type ────────────────────────────────────────────────
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# ── collect sources ───────────────────────────────────────────
file(GLOB_RECURSE CAT_SOURCES
    "src/*.cpp"
)

# ── define the library target ─────────────────────────────────
add_library(catacomb STATIC ${CAT_SOURCES})

# ── include directories ───────────────────────────────────────
target_include_directories(catacomb PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ── compiler warnings ─────────────────────────────────────────
target_compile_options(catacomb PRIVATE
    -Wall
    -Wextra
    -Wconversion
    -Wunreachable-code
    -Wno-unused-parameter
    -Wpointer-arith
    -fpic
)

# ── compile definitions per build type ───────────────────────
target_compile_definitions(catacomb PRIVATE
    $<$<CONFIG:Debug>:DEVELOP ENABLE_BENCHMARK>
    $<$<CONFIG:Develop>:DEVELOP ENABLE_BENCHMARK>
    LINUX
)

# ── optimization flags per build type ────────────────────────
target_compile_options(catacomb PRIVATE
    $<$<CONFIG:Debug>:-g -O0>
    $<$<CONFIG:Develop>:-g -O1> 
    $<$<CONFIG:Release>:-O3>           
)

# ── subdirectory ──────────────────────────────────────────────
add_subdirectory(sandbox)
