cmake_minimum_required(VERSION 3.14.5)
project(RayTracer)

set(CMAKE_CXX_STANDARD 14)

add_executable(RayTracer main.cpp basic_objects.cpp types.cpp)
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

target_link_libraries(RayTracer ${OPENGL_LIBRARIES})