cmake_minimum_required(VERSION 3.14)
project(chip8)
set(CMAKE_CXX_STANDARD 11)

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
        # Include SDL implementation files
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    )
    
    target_include_directories(imgui PUBLIC 
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
        ${SDL2_INCLUDE_DIRS}
    )
    
    # Link with SDL2 libraries
    if(SDL2_FOUND)
        target_link_libraries(imgui PUBLIC ${SDL2_LIBRARIES})
    else()
        target_link_libraries(imgui PUBLIC SDL2-static)
    endif()
endif()

message(STATUS "Checking source files:")
foreach(src_file src/Chip8.cpp src/Main.cpp src/Opcodes.cpp src/Platform.cpp)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${src_file}")
        message(STATUS "  ${src_file} - Found")
    else()
        message(STATUS "  ${src_file} - NOT FOUND")
    endif()
endforeach()

# Define your executable
add_executable(
    chip8
    src/Chip8.cpp
    src/Main.cpp
    src/Opcodes.cpp
    src/Platform.cpp
)

# Add include directories
target_include_directories(chip8 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${SDL2_INCLUDE_DIRS}
)

# Compiler options
target_compile_options(chip8 PRIVATE -Wall)

# Link libraries
target_link_libraries(chip8 PRIVATE 
    glad 
    imgui
)

# Link SDL2 libraries
if(SDL2_FOUND)
    target_link_libraries(chip8 PRIVATE ${SDL2_LIBRARIES})
else()
    target_link_libraries(chip8 PRIVATE SDL2-static)
endif()

# Print some debug info at configure time
message(STATUS "SDL2 Include Dirs: ${SDL2_INCLUDE_DIRS}")
if(SDL2_FOUND)
    message(STATUS "SDL2 Libraries: ${SDL2_LIBRARIES}")
endif()