# cg01-intersection

To build just run the build script with the command: ./build.sh
Please notice that you must have:

1. cmake installed (min version 3.3)
2. make installed
3. opengl, glu and glut installed

In case that the script fails, try these steps:

1. mkdir build    // creates an empty dir
2. cd build
3. cmake ..
4. make
5. ./cg01_intersection

If both of the methods above fail, just compile the main.cpp source file with your preferred compiler.
Do not forget to use opengl, glut and glu dependancies. Commands below (using g++):

Linux: g++ main.cpp -o cg01-intersection -std=c++11 -lglut -lGLU -lGL
OSX: g++ main.cpp -o cg01-intersection -std=c++11 -framework OpenGL -framework GLUT
