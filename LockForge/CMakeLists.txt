# CMakeList.txt : CMake project for LockForge, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (LockForge "LockForge.cpp" "LockForge.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET LockForge PROPERTY CXX_STANDARD 23)
endif()

# TODO: Add tests and install targets if needed.
