cmake_minimum_required(VERSION 2.8)
project(cg01_intersection)

add_executable(cg01_intersection main.cpp)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} )

target_link_libraries(cg01_intersection ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )