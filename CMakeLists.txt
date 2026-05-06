# Check version
cmake_minimum_required(VERSION 3.25)

# Set project name
project(barOP C CXX)

# Find necessary VTK components
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
  message(FATAL_ERROR "Unable to find VTK build folder(s).")
endif()

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Generate shared libraries (dynamic)
add_library(trussStructure SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/trussStructure.cpp
                                  ${CMAKE_CURRENT_SOURCE_DIR}/src/trussElement.cpp)

# Generate executable
add_executable(barOP ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/trussVis.cpp)

add_library(mathflb ${CMAKE_CURRENT_SOURCE_DIR}/src/mathflb.cpp)

# Link libraries
target_link_libraries(barOP PRIVATE trussStructure mathflb ${VTK_LIBRARIES})

# vtk_module_autoinit is needed
vtk_module_autoinit(
  TARGETS barOP
  MODULES ${VTK_LIBRARIES}
)

# Tell CMake there is a separete CMakeLists.txt in ./tests
include(${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt)
