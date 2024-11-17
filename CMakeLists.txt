# Minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Project name and language
project(clox LANGUAGES C)

# Specify the C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set output directories for build artifacts
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Include directories
include_directories(src)

# Add source files
file(GLOB_RECURSE SOURCES "src/*.c")
# Define the executable
add_executable(clox ${SOURCES})

# Link libraries if needed (e.g., pthread, m) target_link_libraries(MyProject
# pthread m)
