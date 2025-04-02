get_filename_component(CMAKE_CURRENT_PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)

message(STATUS "Checking source files:")

# target_compile_options(chip8 PRIVATE -Wall)
if(ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        message(STATUS "Enabling coverage build...")

        # Add necessary compiler and linker flags for coverage
        add_compile_options(--coverage -fprofile-arcs -ftest-coverage)
        add_link_options(--coverage)

        # Ensure CMake does not optimize or strip debugging info
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} --coverage")
        set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --coverage")

    else()
        message(WARNING "Code coverage is only supported with GCC or Clang.")
    endif()
endif()

file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# set(SRC_FILES 
#     src/Chip8.cpp src/Opcodes.cpp src/Platform.cpp)

# if a needed folder must be found at all cost 
# =>
# foreach(src_file ${SRC_FILES})
#     if(EXISTS "${src_file}")
#         message(STATUS "  ${src_file} - Found")
#     else()
#         message(STATUS "  ${src_file} - NOT FOUND")
#     endif()
# endforeach()

message(STATUS "Linked chip8 library:")
add_library(chip8_lib
    ${SRC_FILES}
)

target_include_directories(chip8_lib PUBLIC
    ${CMAKE_CURRENT_PARENT_DIR}/include
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

message(STATUS "Linked binary")
add_executable(chip8
    main.cpp
)
add_executable(gro_main
    gro_main.cpp
)

target_link_libraries(chip8 PRIVATE chip8_lib)