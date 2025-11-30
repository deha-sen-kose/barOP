find_package(GTest CONFIG REQUIRED)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add testing
enable_testing()

add_executable(unitTests tests/unitTests.cpp
                        tests/matrixTests.cpp
                        tests/trussElementTests.cpp
                        tests/trussStructureTests.cpp)

target_link_libraries(unitTests PRIVATE

    trussStructure
    GTest::gtest
    # Provides a main function for googletests
    GTest::gtest_main
)

# Register the tests
include(GoogleTest)
gtest_discover_tests(unitTests)
