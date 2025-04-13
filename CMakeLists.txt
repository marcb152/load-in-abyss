cmake_minimum_required(VERSION 3.30)
project(TempCpp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(libs/glfw)
add_subdirectory(libs/bgfx.cmake)

bgfx_compile_shaders(
        TYPE VERTEX
        SHADERS vs_cubes.sc
        VARYING_DEF ${CMAKE_SOURCE_DIR}/varying.def.sc
        OUTPUT_DIR ${CMAKE_BINARY_DIR}/shaders
        INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/libs/bgfx.cmake/bgfx/src
)
bgfx_compile_shaders(
        TYPE FRAGMENT
        SHADERS fs_cubes.sc
        VARYING_DEF ${CMAKE_SOURCE_DIR}/varying.def.sc
        OUTPUT_DIR ${CMAKE_BINARY_DIR}/shaders
        INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/libs/bgfx.cmake/bgfx/src
)

file(GLOB SOURCES *.cpp)
add_executable(TempCpp ${SOURCES} vs_cubes.sc fs_cubes.sc)

target_link_libraries(TempCpp glfw bgfx bimg bx)
target_include_directories(TempCpp PRIVATE
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/bgfx.cmake"
        "${PROJECT_SOURCE_DIR}/glfw"
        "${CMAKE_BINARY_DIR}/shaders"
)
