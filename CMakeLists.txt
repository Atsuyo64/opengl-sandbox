cmake_minimum_required(VERSION 3.16)
set(EXPORT_COMPILE_COMMANDS 1)
project(sandbox)

set(CMAKE_CXX_STANDARD 17)
option(STATIC_GLFW "Compile glfw locally" ON)

if(STATIC_GLFW)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory(vendored/glfw)
else()
    link_libraries(GL glfw)
endif(STATIC_GLFW)

if(WIN32)
    link_libraries(opengl32)
endif(WIN32)

include_directories(
    include
    vendored/glad
    vendored/imgui
    vendored/imgui/backends
)

FILE(GLOB SRC_FILES src/**.c src/**.cc src/**.cpp)
#to add source files manually:
#set(SRC_FILES
#   src/main.c
#)

set(IMGUI_SOURCES
vendored/imgui/imgui.cpp
vendored/imgui/imgui_demo.cpp
vendored/imgui/imgui_draw.cpp
vendored/imgui/imgui_tables.cpp
vendored/imgui/imgui_widgets.cpp
vendored/imgui/backends/imgui_impl_opengl3.cpp
vendored/imgui/backends/imgui_impl_glfw.cpp
)

add_library(glad vendored/glad/glad.c)
    
add_executable(${CMAKE_PROJECT_NAME} src/main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} glad glfw)
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${SRC_FILES} ${IMGUI_SOURCES})