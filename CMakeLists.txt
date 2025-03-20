message(STATUS "======= COMPILATION PIPELINE CHECKS ======")

cmake_minimum_required(VERSION 3.14)
project(chip8)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

message(STATUS "======= FETCHING DEPENDENCIES ======")

# Include FetchContent for libraries that need to be fetched
include(FetchContent)

# Find SDL2 using pkg-config (common on Linux)
find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(SDL2 QUIET sdl2)
endif()

# Variable to hold our SDL2 libraries info
if(SDL2_FOUND)
    message(STATUS "Found SDL2 via pkg-config")
    # No need to set SDL2_TARGET as we'll use variables provided by pkg-config
else()
    message(STATUS "System SDL2 not found. Building from source...")
    
    # If SDL2 is not found, download and build it
    FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.0.20
        CMAKE_ARGS 
            -DSDL_SHARED=OFF 
            -DSDL_TEST=OFF 
            -DSDL_STATIC=ON
    )
    FetchContent_MakeAvailable(SDL2)
    # Set SDL2 include and lib vars manually
    set(SDL2_INCLUDE_DIRS "${SDL2_SOURCE_DIR}/include" "${SDL2_BINARY_DIR}/include")
    set(SDL2_LIBRARIES SDL2-static)
endif()
message(STATUS "SDL2 Libraries: ${SDL2_LIBRARIES}")

# Fetch GLAD
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v0.1.36
)
FetchContent_MakeAvailable(glad)

# Fetch ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.88
)
FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
    FetchContent_Populate(imgui)
    
    # ImGui doesn't have a CMakeLists.txt, so we need to create a target for it
    add_library(imgui STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    )
    
    target_include_directories(imgui PUBLIC 
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
        ${SDL2_INCLUDE_DIRS}
    )
    
    if(SDL2_FOUND)
        target_link_libraries(imgui PUBLIC ${SDL2_LIBRARIES})
    else()
        target_link_libraries(imgui PUBLIC SDL2-static)
    endif()
endif()

message(STATUS "====== BUILDING PROJECT ======")

message(STATUS "Checking source files:")

file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# set(SRC_FILES 
#     src/Chip8.cpp src/Opcodes.cpp src/Platform.cpp)

# if a needed folder must be found at all cost 
# =>
# foreach(src_file ${SRC_FILES})
#     if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${src_file}")
#         message(STATUS "  ${src_file} - Found")
#     else()
#         message(STATUS "  ${src_file} - NOT FOUND")
#     endif()
# endforeach()

message(STATUS "Building chip8 library:")
add_library(chip8_lib
    ${SRC_FILES}
)

target_include_directories(chip8_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(chip8_lib PUBLIC 
    glad 
    imgui
)

if(SDL2_FOUND)
    target_link_libraries(chip8_lib PUBLIC ${SDL2_LIBRARIES})
else()
    target_link_libraries(chip8_lib PUBLIC SDL2-static)
endif()

message(STATUS "Build binary")
add_executable(chip8
    src/main.cpp
)

target_compile_options(chip8 PRIVATE -Wall)

target_link_libraries(chip8 PRIVATE chip8_lib)

message(STATUS "====== BUILDING TESTS ======")

file(GLOB_RECURSE TEST_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp"
)

# Testing environment setup (GoogleTest)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.11.0  # Use a stable version or any other stable release
)
FetchContent_MakeAvailable(googletest)

# Define the test executable
add_executable(chip8_tests
    ${TEST_FILES}
    # tests/test_main.cpp // TODO: consider adding a whole SW test (main)
)

target_include_directories(chip8_tests PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/include/
)

file(GLOB_RECURSE TEST_INCLUDE_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} tests/include/*)

foreach(dir ${TEST_INCLUDE_DIRS})
    get_filename_component(parent_dir ${dir} DIRECTORY)
    target_include_directories(chip8_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/${parent_dir})
endforeach()

# Link the test library and other dependencies
target_link_libraries(chip8_tests PRIVATE 
    chip8_lib gtest gtest_main
)

# Register the tests with CTest
enable_testing()
# add_test(NAME Chip8Tests COMMAND chip8_tests)
add_test(NAME Chip8Tests COMMAND chip8_tests)

