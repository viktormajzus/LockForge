cmake_minimum_required(VERSION 3.20)
project(LockForge VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(WIN32)
    add_compile_definitions(_WIN32)
elseif(APPLE)
    add_compile_definitions(__APPLE__)
elseif(UNIX)
    add_compile_definitions(__linux__)
endif()

add_executable(LockForge 
    src/main.cpp
    src/PathHandler.cpp
)

target_include_directories(LockForge PRIVATE include)

