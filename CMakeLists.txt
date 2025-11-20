# Check version
cmake_minimum_required(VERSION 4.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_custom_target(copy_compile_commands ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json
    COMMENT "Copying compile_commands.json to project root"
    VERBATIM
)

# Make the copy happen after ALL other build targets
add_dependencies(copy_compile_commands ${ALL_BUILD_TARGETS})

# Define Compilers for C and C++
# set(CMAKE_C_COMPILER gcc)
# set(CMAKE_CXX_COMPILER g++)

# Set project name
project(barOP CXX)

# Set build options
set(CMAKE_CXX_FLAGS "-g") # debug mode
set(CMAKE_CXX_FLAGS "-O2 -march=native -std=c++17 ") # release mode

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Generate shared libraries (dynamic)
add_library(bar3Dmesh SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/Bar3DMesh.cpp)
# add_library(matrix SHARED ${CMAKE_CURRENT_SOURCE_DIR}/include/math/Matrix.h)

# Generate executable
add_executable(barOP ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

# Link libraries
target_link_libraries(barOP bar3Dmesh)

# Tell CMake there is a separete CMakeLists.txt in ./tests
# add_subdirectory(tests)

find_package(GTest CONFIG REQUIRED)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add testing
enable_testing()

add_executable(unitTests ${CMAKE_CURRENT_SOURCE_DIR}/tests/unitTests.cpp)

target_link_libraries(unitTests PRIVATE

    bar3Dmesh
    GTest::gtest
    # Provides a main function for googletests
    GTest::gtest_main

)

# Register the tests
include(GoogleTest)
gtest_discover_tests(unitTests)
