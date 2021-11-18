#version 330 core
//pos variable corresponds to the VBO with the vertices we have previously bound to location 0. 
//These coordinates are passed on unmodified to an internal OpenGL variable that tells the GPU where the vertex is.
layout (location = 0) in vec3 pos;
 
void main()
{
    gl_Position = vec4(pos.xyz, 1.0);
}