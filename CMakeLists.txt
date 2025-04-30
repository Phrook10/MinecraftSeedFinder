cmake_minimum_required(VERSION 3.15)

# Set the project name and C++ standard
project(MinecraftSeedFinder)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# Tell CMake to build cubiomes as static lib
set(BUILD_SHARED_LIBS OFF)

# Include the cubiomes directory
add_subdirectory(cubiomes)

# Include the directories for your custom headers (config.h, seedfinder.h, etc.)
include_directories(
    ${CMAKE_SOURCE_DIR}  # This is to include the root directory (config.h, seedfinder.h)
    cubiomes             # This ensures the cubiomes headers are included
)

# Add the source files for the project (main.cpp, seedfinder.cpp, etc.)
add_executable(seedfinder main.cpp config.cpp seedfinder.cpp interface.cpp)

# Link the cubiomes library with your project
#target_link_libraries(seedfinder PRIVATE cubiomes)


# Link the static library to your project
target_link_libraries(seedfinder PRIVATE cubiomes_static)