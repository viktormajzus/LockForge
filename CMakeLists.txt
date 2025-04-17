# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.20)
project(LockForge LANGUAGES CXX)

# Please use C++ 23 when compiling this project
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Platform macros
if(WIN32)
	add_compile_definitions(LOCKFORGE_PLATFORM_WINDOWS)
elseif(UNIX)
	add_compile_definitions(LOCKFORGE_PLATFORM_UNIX)
endif()

# Header and source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "include/*.h")

add_executable(LockForge ${SOURCES})
target_sources(LockForge PRIVATE ${HEADERS})

target_include_directories(LockForge PRIVATE ${CMAKE_SOURCE_DIR}/include)

find_package(Botan REQUIRED)
target_link_libraries(LockForge PRIVATE Botan::Botan)

# Compiler warnings
if (MSVC)
  target_compile_options(LockForge PRIVATE /W4 /permissive-)
else()
  target_compile_options(LockForge PRIVATE -Wall -Wextra -pedantic -Wno-unused-parameter)
endif()
