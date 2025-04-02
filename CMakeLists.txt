#TODO: handle better the dependencies to avoid installing everything on the computer
message(STATUS "======= COMPILATION PIPELINE CHECKS ======")
cmake_minimum_required(VERSION 3.14)

project(Chip8
    VERSION 0.3
    DESCRIPTION "This repository contains a CHIP-8 emulator, which emulates the CHIP-8 virtual machine architecture used in the 1970s and 1980s for programming simple games. The emulator can load and execute CHIP-8 games, offering a window into early low-level programming and emulator development."
    LANGUAGES CXX
    HOMEPAGE_URL https://github.com/n2oblife/CHIP-8_emulator
    )

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(TARGET ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/chip8)
set(TARGET_TESTS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests/chip8_tests)
set(CMAKE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(CMAKE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(CMAKE_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)
set(README_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)


if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "debug")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "minsizerel")
    set(CMAKE_BUILD_TYPE MinSizeRel CACHE STRING "Build type" FORCE)
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(ENABLE_TESTS ON CACHE BOOL "Enable Chip8 Emulator Tests" FORCE)
    set(ENABLE_COVERAGE ON CACHE BOOL "Enable code coverage analysis" FORCE)
    # Enable STATUSs for debug
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(-Wall -Wextra -Wpedantic -o0 -g)
    elseif (MSVC)
        add_compile_options(/W4 /WX)
    endif()
else()
    set(ENABLE_COVERAGE OFF CACHE BOOL "Enable code coverage analysis" FORCE)
endif()


message(STATUS "======= FETCHING DEPENDENCIES ======")

# Include FetchContent for libraries that need to be fetched
include(FetchContent)
include(ExternalProject)
find_package(Threads REQUIRED)

find_package(PkgConfig)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(SDL2 sdl2)
endif()
# Variable to hold our SDL2 libraries info
if(SDL2_FOUND)
    message(STATUS "Found SDL2 via pkg-config")
    # No need to set SDL2_TARGET as we'll use variables provided by pkg-config
else()
    message(STATUS "System SDL2 not found. Fetch from source...")
    
    # If SDL2 is not found, download and fetch it
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
    message(STATUS "SDL2 Libraries fetched: ${SDL2_LIBRARIES}")
endif()

# Fetch GLAD
if(PKG_CONFIG_FOUND)
    pkg_check_modules(GLAD glad)
endif()
# Variable to hold our SDL2 libraries info
if(GLAD_FOUND)
    message(STATUS "Found GLAD via pkg-config")
    # No need to set GLAD_TARGET as we'll use variables provided by pkg-config
else()
    message(STATUS "System GLAD not found. Fetching from source...")
    
    FetchContent_Declare(
        glad
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git
        GIT_TAG v0.1.36
    )
    FetchContent_MakeAvailable(glad)

    set(GLAD_INCLUDE_DIRS "${GLAD_SOURCE_DIR}/include" "${GLAD_BINARY_DIR}/include")
    set(GLAD GLAD-static)
    message(STATUS "GLAD Libraries fetched: ${GLAD_LIBRARIES}")
endif()

# Fetch ImGui
if(PKG_CONFIG_FOUND)
    pkg_check_modules(IMGUI imgui)
endif()
# Variable to hold our SDL2 libraries info
if(IMGUI_FOUND)
    message(STATUS "Found IMGUI via pkg-config")
    # No need to set IMGUI_TARGET as we'll use variables provided by pkg-config
else()
    message(STATUS "System IMGUI not found. Fetching from source...")

    # Fetch ImGui
    FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.88
    )
    
    FetchContent_GetProperties(imgui)
    # if(NOT imgui_POPULATED)
        FetchContent_Populate(imgui)
        
        # ImGui doesn't have a CMakeLists.txt, so we need to create a target for it
        file(GLOB_RECURSE IMGUI_SRC_FILES "${imgui_SOURCE_DIR}/*.cpp")
        add_library(imgui STATIC
            # ${IMGUI_SRC_FILES}
            ${imgui_SOURCE_DIR}/imgui.cpp
            ${imgui_SOURCE_DIR}/imgui.h
            ${imgui_SOURCE_DIR}/imgui_demo.cpp
            ${imgui_SOURCE_DIR}/imgui_draw.cpp
            ${imgui_SOURCE_DIR}/imgui_tables.cpp
            ${imgui_SOURCE_DIR}/imgui_widgets.cpp
            ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl.cpp
            # ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
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
    # endif()

    set(IMGUI_INCLUDE_DIRS "${IMGUI_SOURCE_DIR}/include" "${IMGUI_BINARY_DIR}/include")
    set(IMGUI IMGUI-static)
    message(STATUS "IMGUI libraries fetched: ${imgui_SOURCE_DIR}")
endif()


find_package(Doxygen QUIET)
if(NOT DOXYGEN_FOUND)
    message(STATUS "Doxygen not found. Fetching from source...")
    
    FetchContent_Declare(
        doxygen
        GIT_REPOSITORY https://github.com/doxygen/doxygen.git
        GIT_TAG Release_1_10_0 # Use the latest stable release tag
    )
    
    FetchContent_MakeAvailable(doxygen)
    
    # Set the newly built Doxygen as the executable
    set(DOXYGEN_EXECUTABLE ${CMAKE_BINARY_DIR}/_deps/doxygen-build/bin/doxygen)
    
    if(EXISTS ${DOXYGEN_EXECUTABLE})
        message(STATUS "Doxygen successfully built: ${DOXYGEN_EXECUTABLE}")
    else()
        message(FATAL_ERROR "Failed to build Doxygen. Documentation won't be generated.")
    endif()
endif()

if(DOXYGEN_EXECUTABLE)
    message(STATUS "Doxygen executable: ${DOXYGEN_EXECUTABLE}")

    set(DOXYGEN_INPUT_DIR "${CMAKE_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}")
    set(DOXYGEN_OUTPUT_DIR ${CMAKE_BINARY_DIR}/docs)
    set(DOXYGEN_CONFIG_FILE ${CMAKE_BINARY_DIR}/docs/Doxyfile.in)
    
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in ${DOXYGEN_CONFIG_FILE} @ONLY)

    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CONFIG_FILE}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/docs
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )

endif()

message(STATUS "====== LINKING PROJECT ======")
add_subdirectory(${CMAKE_SOURCE_DIR})


if(ENABLE_TESTS)
    message(STATUS "====== LINKING TESTS ======")
    add_subdirectory(${CMAKE_TEST_DIR})
endif()

# Installation (Optional)
# install(TARGETS chip8 DESTINATION bin)