cmake_minimum_required(VERSION 3.15)
project(MinecraftSeedFinder)

set(CMAKE_CXX_STANDARD 17)

# Add Cubiomes C code
add_subdirectory(cubiomes)

# Your executable
add_executable(seedfinder main.cpp)
target_include_directories(seedfinder PRIVATE cubiomes)
target_link_libraries(seedfinder PRIVATE cubiomes)
