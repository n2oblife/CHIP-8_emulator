# Set the minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Chip8Emulator VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include /usr/include)

# Find and link SDL2 library
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Add source files
file(GLOB SRC_FILES src/*.cpp)

# Add the executable target
add_executable(chip8_emulator ${SRC_FILES})

# Link SDL2 library to the executable
target_link_libraries(chip8_emulator ${SDL2_LIBRARIES})

# Add compiler flags for warnings
target_compile_options(chip8_emulator PRIVATE -Wall -Wextra)

# Set the output directories
set_target_properties(chip8_emulator PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    OUTPUT_NAME "chip8_emulator"
)

# Custom clean target to remove build artifacts
add_custom_target(clean_all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E remove_directory lib_build
)
