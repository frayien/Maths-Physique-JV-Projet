cmake_minimum_required(VERSION 3.0.0)
project(Proj VERSION 0.1.0)

### CMake config ###
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_C_COMPILER "gcc")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../")

### Project headers ###
include_directories(/mingw64/include)
include_directories(include)

### Dependencies ###

### Project sources ###
file(GLOB_RECURSE SRCS src/*)
file(GLOB_RECURSE HEADERS include/*)

add_executable(Proj ${HEADERS} ${SRCS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_link_libraries(
    Proj
    glfw3
    vulkan
    VkLayer_khronos_validation
)
