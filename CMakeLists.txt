cmake_minimum_required(VERSION 3.20)

project(LockForge VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_USE_CXX11_ABI=0")

include(FetchContent)

# Download and build Argon2
FetchContent_Declare(
    argon2
    GIT_REPOSITORY https://github.com/P-H-C/phc-winner-argon2.git
    GIT_TAG master
)

FetchContent_MakeAvailable(argon2)

add_library(argon2 STATIC
    ${argon2_SOURCE_DIR}/src/argon2.c
    ${argon2_SOURCE_DIR}/src/core.c
    ${argon2_SOURCE_DIR}/src/blake2/blake2b.c
    ${argon2_SOURCE_DIR}/src/thread.c
    ${argon2_SOURCE_DIR}/src/encoding.c
    ${argon2_SOURCE_DIR}/src/ref.c
)

target_include_directories(argon2 PUBLIC ${argon2_SOURCE_DIR}/include)

# Glob all source files in LockForge
file(GLOB_RECURSE LOCK_FORGE_SRC CONFIGURE_DEPENDS src/*.cpp)

# Add the LockForge executable
add_executable(LockForge ${LOCK_FORGE_SRC})

# Include the Argon2 headers and include/ directory headers
target_include_directories(LockForge PRIVATE 
    include 
    ${argon2_SOURCE_DIR}/include
)

# Make sure Argon2 builds before LockForge
add_dependencies(LockForge argon2)

# Link Argon2 with LockForge
target_link_libraries(LockForge PRIVATE argon2 stdc++fs)
