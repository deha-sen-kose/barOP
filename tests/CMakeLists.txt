find_package(GTest CONFIG REQUIRED)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add testing
enable_testing()

add_executable(unitTests tests/unitTests.cpp tests/barMeshTests.cpp tests/matrixTests.cpp)

target_link_libraries(unitTests PRIVATE

    barMesh
    GTest::gtest
    # Provides a main function for googletests
    GTest::gtest_main
)

# Register the tests
include(GoogleTest)
gtest_discover_tests(unitTests)
