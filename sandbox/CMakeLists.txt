# ── find SDL2 ─────────────────────────────────────────────────
find_package(SDL2 REQUIRED)

# ── collect sandbox sources ───────────────────────────────────
file(GLOB_RECURSE SANDBOX_SOURCES
    "src/*.cpp"
    "src/*.c"
)

# ── define the executable ─────────────────────────────────────
add_executable(main ${SANDBOX_SOURCES})

# ── link libraries ────────────────────────────────────────────
target_link_libraries(main PRIVATE
    catacomb                             
    SDL2::SDL2                           
    m                                    
    ${CMAKE_SOURCE_DIR}/lib/glad.a       
)

# ── sanitizers for debug builds ──────────────────────────────
target_compile_options(main PRIVATE
    $<$<CONFIG:Debug>:-fsanitize=address,undefined -fno-omit-frame-pointer>
)
target_link_options(main PRIVATE
    $<$<CONFIG:Debug>:-fsanitize=address,undefined>
)

# ── compile definitions ───────────────────────────────────────
target_compile_definitions(main PRIVATE
    $<$<CONFIG:Debug>:DEVELOP ENABLE_BENCHMARK>
    $<$<CONFIG:Develop>:DEVELOP ENABLE_BENCHMARK>
)

target_compile_options(main PRIVATE
    -Wall -Wextra -Wconversion -Wunreachable-code
    -Wno-unused-parameter -Wpointer-arith
    $<$<CONFIG:Debug>:-g -O0>
    $<$<CONFIG:Develop>:-g -O1>
    $<$<CONFIG:Release>:-O3>
)
