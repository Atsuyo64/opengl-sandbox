cmake_minimum_required(VERSION 3.16)
project(sandbox LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
include(FetchContent)

# Options
option(STATIC_GLFW "Build GLFW from vendor directory" ON)
option(INCLUDE_ASSIMP "Include assimp model importer" OFF)

# Sanitizers in debug mode
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
endif()

add_executable(${CMAKE_PROJECT_NAME})

# Header only libs
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    vendor/stb
    vendor/glm
)

# GLAD
target_sources(${CMAKE_PROJECT_NAME} PRIVATE vendor/glad/glad.c)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE vendor/glad)

# ImGUI
set(IMGUI_SOURCES
    vendor/imgui/imgui.cpp
    vendor/imgui/imgui_demo.cpp
    vendor/imgui/imgui_draw.cpp
    vendor/imgui/imgui_tables.cpp
    vendor/imgui/imgui_widgets.cpp
    vendor/imgui/backends/imgui_impl_opengl3.cpp
    vendor/imgui/backends/imgui_impl_glfw.cpp
)
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${IMGUI_SOURCES})
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE vendor/imgui)

# GLFW
if(STATIC_GLFW)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory(vendor/glfw)
else()
    find_library(GLFW3 REQUIRED)
endif(STATIC_GLFW)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE glfw)

# assimp (optional)
if(INCLUDE_ASSIMP)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
    set(ASSIMP_BUILD_ASSIMP_VIEW OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
        assimp
        GIT_REPOSITORY https://github.com/assimp/assimp.git
        GIT_TAG v6.0.2
    )
    FetchContent_MakeAvailable(assimp)
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE assimp::assimp)
endif()

# OpenGL
if(WIN32)
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE opengl32)
elseif(APPLE)
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE "-framework OpenGL") # untested
else()
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE GL)
endif(WIN32)

# Main sources
FILE(GLOB_RECURSE PROJECT_SOURCES src/*.c src/*.cc src/*.cpp)
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE src)