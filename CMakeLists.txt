cmake_minimum_required(VERSION 3.30)
project(LoadInAbyss)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Assimp environment variables
set(ASSIMP_NO_EXPORT ON)
set(ASSIMP_BUILD_ZLIB ON)
set(BUILD_SHARED_LIBS OFF)
set(ASSIMP_BUILD_TESTS OFF)

add_subdirectory(libs/glfw)
add_subdirectory(libs/bgfx.cmake)
add_subdirectory(libs/assimp)

# Get all shaders in the shaders directory for each child directory
FILE(GLOB children RELATIVE ${CMAKE_SOURCE_DIR}/res/shaders res/shaders/*)
SET(SHADERS_FILES "")
FOREACH(child ${children})
    IF(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/res/shaders/${child}/)
        message("Shader directory found: shaders/${child}")
        # Get vertex shader (vs) in the ${child} folder
        bgfx_compile_shaders(
                TYPE VERTEX
                SHADERS res/shaders/${child}/vs_${child}.sc
                VARYING_DEF ${CMAKE_SOURCE_DIR}/res/shaders/${child}/varying.def.sc
                OUTPUT_DIR ${CMAKE_BINARY_DIR}/shaders
                INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/libs/bgfx.cmake/bgfx/src
        )
        LIST(APPEND SHADERS_FILES res/shaders/${child}/vs_${child}.sc)
        # Get fragment shader (fs) in the ${child} folder
        bgfx_compile_shaders(
                TYPE FRAGMENT
                SHADERS res/shaders/${child}/fs_${child}.sc
                VARYING_DEF ${CMAKE_SOURCE_DIR}/res/shaders/${child}/varying.def.sc
                OUTPUT_DIR ${CMAKE_BINARY_DIR}/shaders
                INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/libs/bgfx.cmake/bgfx/src
        )
        LIST(APPEND SHADERS_FILES res/shaders/${child}/fs_${child}.sc)
    ENDIF()
ENDFOREACH()

# Copy models to output dir
file(GLOB_RECURSE models LIST_DIRECTORIES false RELATIVE ${CMAKE_SOURCE_DIR}/res/models res/models/*)
foreach(model ${models})
    message("Model found: models/${model}")
    file(COPY ${CMAKE_SOURCE_DIR}/res/models/${model} DESTINATION ${CMAKE_BINARY_DIR}/models)
endforeach()

file(GLOB SOURCES src/*.cpp)
add_executable(LoadInAbyss ${SOURCES} ${SHADERS_FILES})

target_link_libraries(LoadInAbyss PRIVATE
        glfw
        bgfx
        bimg
        bx
        zlibstatic
        assimp
        )
target_include_directories(LoadInAbyss PRIVATE
        "${PROJECT_BINARY_DIR}"
        "${CMAKE_BINARY_DIR}/shaders"
)
