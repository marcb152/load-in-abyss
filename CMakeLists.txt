cmake_minimum_required(VERSION 3.30)
project(TempCpp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(libs/glfw)
add_subdirectory(libs/bgfx.cmake)

# Get all shaders in the shaders directory for each child directory
FILE(GLOB children RELATIVE ${CMAKE_SOURCE_DIR}/shaders shaders/*)
SET(SHADERS_FILES "")
FOREACH(child ${children})
    IF(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/shaders/${child}/)
        message("Shader directory found: shaders/${child}")
        # Get vertex shader (vs) in the ${child} folder
        bgfx_compile_shaders(
                TYPE VERTEX
                SHADERS shaders/${child}/vs_${child}.sc
                VARYING_DEF ${CMAKE_SOURCE_DIR}/shaders/${child}/varying.def.sc
                OUTPUT_DIR ${CMAKE_BINARY_DIR}/shaders
                INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/libs/bgfx.cmake/bgfx/src
        )
        LIST(APPEND SHADERS_FILES shaders/${child}/vs_${child}.sc)
        # Get fragment shader (fs) in the ${child} folder
        bgfx_compile_shaders(
                TYPE FRAGMENT
                SHADERS shaders/${child}/fs_${child}.sc
                VARYING_DEF ${CMAKE_SOURCE_DIR}/shaders/${child}/varying.def.sc
                OUTPUT_DIR ${CMAKE_BINARY_DIR}/shaders
                INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/libs/bgfx.cmake/bgfx/src
        )
        LIST(APPEND SHADERS_FILES shaders/${child}/fs_${child}.sc)
    ENDIF()
ENDFOREACH()

file(GLOB SOURCES *.cpp)
add_executable(TempCpp ${SOURCES} ${SHADERS_FILES})

target_link_libraries(TempCpp glfw bgfx bimg bx)
target_include_directories(TempCpp PRIVATE
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/bgfx.cmake"
        "${PROJECT_SOURCE_DIR}/glfw"
        "${CMAKE_BINARY_DIR}/shaders"
)
