# Mandelbort-Set done in OpenGL

### Steps to build and run  ( program tested only on Linux-Ubuntu 18.04,20.04 )
- install the necessary packages- glut,glfw,glm,glu,glew ( Follow an online documentation to install opengl ).
- open a terminal in the working directory
- execute _g++ main.cpp -o mandel -lGL -lGLU -lglut -lGLEW -lglfw_
- _./mandel_

### Controls 
- Zoom in : _RIGHT_CTRL_
- Zoom out : _LEFT_SHIFT_
- _Arrow keys_ for movement.
- _ESC_ to close the glfw window.

### General info
- To change the colors , go to _shader.frag_ and change the colors based on the comments.
- to increase or decrease the default view, change the necessary value in the _real_ and _img_ computing section of the _shader.frag_. ( comments provided for easy understanding.
