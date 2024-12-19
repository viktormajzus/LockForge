cmake_minimum_required(VERSION 3.20)

project(LockForge VERSION 0.1.0 LANGUAGES C CXX)

# --- CMake settings ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# --- Include FetchContent to fetch dependencies from Git ---
include(FetchContent)

# --- Use vcpkg on Windows, system paths on Linux/Mac ---
if(WIN32)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()

# --- Use FindPackage to locate OpenSSL ---
find_package(OpenSSL REQUIRED)

# --- Use pre-downloaded OpenSSL if available (for users with external dependencies) ---
set(FETCHCONTENT_SOURCE_DIR_OPENSSL ${CMAKE_SOURCE_DIR}/external/_deps/openssl-src)

# --- Fetch Argon2 ---
FetchContent_Declare(
    argon2
    GIT_REPOSITORY https://github.com/P-H-C/phc-winner-argon2.git
    GIT_TAG master
)
FetchContent_MakeAvailable(argon2)

# --- Add Argon2 library as a proper target ---
add_library(argon2 STATIC 
    ${argon2_SOURCE_DIR}/src/argon2.c 
    ${argon2_SOURCE_DIR}/src/core.c 
    ${argon2_SOURCE_DIR}/src/encoding.c 
    ${argon2_SOURCE_DIR}/src/ref.c 
    ${argon2_SOURCE_DIR}/src/thread.c 
    ${argon2_SOURCE_DIR}/src/blake2/blake2b.c 
)

# Include Argon2 headers for LockForge
target_include_directories(argon2 PUBLIC ${argon2_SOURCE_DIR}/include)

# --- Add LockForge executable ---
file(GLOB_RECURSE LOCK_FORGE_SRC CONFIGURE_DEPENDS src/*.cpp)

add_executable(LockForge ${LOCK_FORGE_SRC})

# --- Include directories for LockForge headers ---
target_include_directories(LockForge PRIVATE include)

# --- Link Argon2 and OpenSSL with LockForge ---
target_link_libraries(LockForge PRIVATE argon2 OpenSSL::SSL OpenSSL::Crypto)
