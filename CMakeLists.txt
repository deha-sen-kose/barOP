# Check version
cmake_minimum_required(VERSION 3.25)

#set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#add_custom_target(copy_compile_commands ALL
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different
#       ${CMAKE_BINARY_DIR}/compile_commands.json
#        ${CMAKE_SOURCE_DIR}/compile_commands.json
#    COMMENT "Copying compile_commands.json to project root"
#    VERBATIM
#)

# Make the copy happen after ALL other build targets
#add_dependencies(copy_compile_commands ${ALL_BUILD_TARGETS})

# Define Compilers for C and C++
# set(CMAKE_C_COMPILER gcc)
# set(CMAKE_CXX_COMPILER g++)

# Set project name
project(barOP C CXX)

# find_package(MPI REQUIRED)

find_package(VTK COMPONENTS
  CommonColor
  CommonCore
  CommonDataModel
  InteractionStyle
  RenderingContextOpenGL2
  RenderingCore
  RenderingFreeType
  RenderingGL2PSOpenGL2
  RenderingOpenGL2
  RenderingAnnotation
  FiltersSources
  InteractionWidgets
)

if (NOT VTK_FOUND)
  message(FATAL_ERROR "ColoredLines: Unable to find the VTK build folder.")
endif()

# Set build options
# set(CMAKE_CXX_FLAGS "-g") # debug mode
# set(CMAKE_CXX_FLAGS "-O2 -march=native -std=c++17 ") # release mode

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Generate shared libraries (dynamic)
add_library(trussStructure SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/trussStructure.cpp
                                  ${CMAKE_CURRENT_SOURCE_DIR}/src/trussElement.cpp)

# Generate executable
add_executable(barOP ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/trussVis.cpp)

# Link libraries
target_link_libraries(barOP PRIVATE trussStructure ${VTK_LIBRARIES})

# vtk_module_autoinit is needed
vtk_module_autoinit(
  TARGETS barOP
  MODULES ${VTK_LIBRARIES}
)

# Tell CMake there is a separete CMakeLists.txt in ./tests
include(${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt)
