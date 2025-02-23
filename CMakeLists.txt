cmake_minimum_required(VERSION 3.16)
set(EXPORT_COMPILE_COMMANDS 1)
project(sandbox)

set(CMAKE_CXX_STANDARD 17)
option(STATIC_GLFW "Compile glfw locally" ON)

if(STATIC_GLFW)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory(libs/glfw)
else()
    link_libraries(glfw)
endif(STATIC_GLFW)

if(WIN32)
    link_libraries(opengl32)
else()
    link_libraries(GL)
endif(WIN32)

include_directories(
    include
    libs/glad
    libs/imgui
    libs/imgui/backends
)

#add_compile_options($<$<CONFIG:Debug>:-fsanitize=address>)
#add_link_options($<$<CONFIG:Debug>:-fsanitize=address>)

FILE(GLOB SRC_FILES src/**.c src/**.cc src/**.cpp)
#to add source files manually:
#set(SRC_FILES
#   src/main.c
#)

set(IMGUI_SOURCES
libs/imgui/imgui.cpp
libs/imgui/imgui_demo.cpp
libs/imgui/imgui_draw.cpp
libs/imgui/imgui_tables.cpp
libs/imgui/imgui_widgets.cpp
libs/imgui/backends/imgui_impl_opengl3.cpp
libs/imgui/backends/imgui_impl_glfw.cpp
)

add_library(glad libs/glad/glad.c)
    
add_executable(${CMAKE_PROJECT_NAME} src/main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} glad glfw)
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${SRC_FILES} ${IMGUI_SOURCES})