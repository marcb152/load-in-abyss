cmake_minimum_required(VERSION 3.30)
project(TempCpp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(libs/glfw)
add_subdirectory(libs/bgfx.cmake)

add_executable(TempCpp main.cpp)
target_link_libraries(TempCpp glfw bgfx bimg bx)
target_include_directories(TempCpp PRIVATE
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/bgfx.cmake"
        "${PROJECT_SOURCE_DIR}/glfw"
)
