set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/tests)

if(PKG_CONFIG_FOUND)
    pkg_check_modules(GTEST gtest>=1.11.0)
    pkg_check_modules(GMOCK gmock>=1.11.0)
endif()

if(GTEST_FOUND)
    message(STATUS "Found GTEST via pkg-config")
    # No need to set GTEST_TARGET as we'll use variables provided by pkg-config
else()
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.11.0  # Use a stable version or any other stable release
    )
    FetchContent_MakeAvailable(googletest)
    set(GTEST_INCLUDE_DIRS "${GTEST_SOURCE_DIR}/include" "${GTEST_BINARY_DIR}/include")
    set(GTEST GTEST-static)
    message(STATUS "GTEST libraries fetched: ${GTEST_LIBRARIES}")
endif()

file(GLOB_RECURSE TEST_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

# Define the test executable
add_executable(chip8_tests
    ${TEST_FILES}
)

target_include_directories(chip8_tests PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
)

file(GLOB_RECURSE TEST_INCLUDE_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/*)

foreach(dir ${TEST_INCLUDE_DIRS})
    get_filename_component(parent_dir ${dir} DIRECTORY)
    target_include_directories(chip8_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/${parent_dir})
endforeach()

# Link the test library and other dependencies
target_link_libraries(chip8_tests PRIVATE 
    chip8_lib gtest gtest_main
)

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

# Register the tests with CTest
enable_testing()
add_test(NAME Chip8Tests 
    COMMAND chip8_tests 
    # WORKING_DIRECTORY 
    )
