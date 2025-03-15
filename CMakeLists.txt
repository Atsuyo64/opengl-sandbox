cmake_minimum_required(VERSION 3.16)
set(EXPORT_COMPILE_COMMANDS 1)
project(sandbox)

set(CMAKE_CXX_STANDARD 17)
option(STATIC_GLFW "Compile glfw locally" ON)
option(INCLUDE_ASSIMP
    "Include assimp (model/scene importer) in projet (quite heavy/long to compile)"
    ON
)

if(STATIC_GLFW)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory(vendor/glfw)
else()
    link_libraries(glfw)
endif(STATIC_GLFW)

#assimp
if(INCLUDE_ASSIMP)
    if(BUILD_SHARED_LIBS)
        set(PUSHED_BUILD_SHARED_LIBS ON)
    else()
        set(PUSHED_BUILD_SHARED_LIBS OFF)
    endif()
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
    set(ASSIMP_BUILD_ASSIMP_VIEW OFF CACHE BOOL "" FORCE)
    add_subdirectory(vendor/assimp)
    if(PUSHED_BUILD_SHARED_LIBS)
        set(BUILD_SHARED_LIBS ON)
    else()
        set(BUILD_SHARED_LIBS OFF)
    endif()
    unset(PUSHED_BUILD_SHARED_LIBS)
endif()

if(WIN32)
    link_libraries(opengl32)
else()
    link_libraries(GL)
endif(WIN32)

include_directories(
    include
    src
    vendor/glad
    vendor/imgui
    vendor/imgui/backends
    vendor/glm
)

#add_compile_options($<$<CONFIG:Debug>:-fsanitize=address>)
#add_link_options($<$<CONFIG:Debug>:-fsanitize=address>)

#FILE(GLOB SRC_FILES src/**.c src/**.cc src/**.cpp)
set(SRC_FILES
   src/main.cpp
   src/camera.cpp
   src/openglUtils/glDebugLog.cpp
   src/openglUtils/shader.cpp
   src/shapes/spheres.cpp
   src/scenes/pyramid_scene.cpp
   src/scenes/sphere_scene.cpp
)

set(IMGUI_SOURCES
vendor/imgui/imgui.cpp
vendor/imgui/imgui_demo.cpp
vendor/imgui/imgui_draw.cpp
vendor/imgui/imgui_tables.cpp
vendor/imgui/imgui_widgets.cpp
vendor/imgui/backends/imgui_impl_opengl3.cpp
vendor/imgui/backends/imgui_impl_glfw.cpp
)

add_library(glad vendor/glad/glad.c)
    
add_executable(${CMAKE_PROJECT_NAME} src/main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} glad glfw assimp)
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${SRC_FILES} ${IMGUI_SOURCES})