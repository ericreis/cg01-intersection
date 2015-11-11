cmake_minimum_required(VERSION 3.3)
project(cg01_intersection)

if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -framework OpenGL -framework GLUT")
elseif(LINUX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lglut -lGLU -lGL")
endif()


set(SOURCE_FILES main.cpp)
add_executable(cg01_intersection ${SOURCE_FILES})