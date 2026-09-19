cmake_minimum_required(VERSION 3.10)

project(CVoxelEngine)

set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(OpenGL_GL_PREFERENCE "GLVND")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_BUILD_TYPE Debug)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(include)
include_directories(src/h_files)

add_library(glad STATIC src/c_files/glad.c)

file(GLOB_RECURSE C_FILES src/c_files/*.c)
file(GLOB_RECURSE H_FILES src/h_files/*.h)

add_executable(CVoxelEngine ${C_FILES})
target_link_libraries(CVoxelEngine ${OPENGL_LIBRARIES} glad glfw m)
