// Author : Paleti Krishnasai - CED18I039
// g++ main.cpp -o mandel -lGL -lGLU -lglut -lGLEW -lglfw
#include <bits/stdc++.h>
 
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
 
#include "Shader.h"

// default initialize global variables , works best with multiple files and especiallly shaders
int screen_width{ 1080 };
int screen_height{ 1080 };
 
int num_frames{};
float last_time{};
float center_x{ 0.0f };
float center_y{ 0.0f };
float zoom{ 1.0 };

// represent the coordinates of the four corners of the rectangle filling the screen
float vertices[] = 
{
//    x      y      z   
    -1.0f, -1.0f, -0.0f,
     1.0f,  1.0f, -0.0f,
    -1.0f,  1.0f, -0.0f,
     1.0f, -1.0f, -0.0f
};

// ndexes the vertices defining two triangles
unsigned int indices[] = 
{
//  2----,1
//  | ,' |
//  0'---3
    0, 1, 2,
    0, 3, 1
};
 
 // resize the window. 
void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
    glViewport(0, 0, width, height);
}

//  Keyboard controls to view all points 
//  change the subset, the rectangle, of the complex number plane, feed to Mandelbrot algorithm
void process_input(GLFWwindow * window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
 
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        center_y = center_y + 0.01f * zoom;
        if (center_y > 1.0f)
        {
            center_y = 1.0f;
        }
    }
 
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        center_y = center_y - 0.01f * zoom;
        if (center_y < -1.0f)
        {
            center_y = -1.0f;
        }
    }
 //
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        center_x = center_x - 0.01f * zoom;
        if (center_x < -1.0f)
        {
            center_x = -1.0f;
        }
    }
 
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        center_x = center_x + 0.01f * zoom;
        if (center_x > 1.0f)
        {
            center_x = 1.0f;
        }
    }
 
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        zoom = zoom * 1.04f;
        if (zoom > 1.0f)
        {
            zoom = 1.0f;
        }
    }
 
    if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
    {
        zoom = zoom * 0.96f;
        if (zoom < 0.00001f)
        {
            zoom = 0.00001f;
        }
    }
}

  // increment the frame count, and each second print out the time
void countFPS()
{
    double current_time = glfwGetTime();
    num_frames++;
    if (current_time - last_time >= 1.0)
    {
        std::cout << 1000.0 / num_frames << "ms / frame\n";
        num_frames = 0;
        last_time += 1.0;
    }
}

/*
 - takes the iteration data as a standard vector of floats, and returns the vec4 representing the color ranges.
 - sort all data. This leaves us with some amount of zeroes (which correspond to pixels in the Mandelbrot set), and non-zero values.
 - search for the first non-zero value at index lowest. 
 -  first bound is this lowest non-zero value.
 - next bound is at 3/4 ( done using trial and error, around 0.7-0.85 has good color contrast, below or above destroys the beauty ).

*/
glm::vec4 find_ranges(std::vector<float> & data)
{
    std::sort(data.begin(), data.end());
    int lowest = 0;
    while (data[lowest] == 0.0f)
    {
        ++lowest;
    }
 
    int size = data.size();
    int length = size - lowest;
    glm::vec4 ranges = glm::vec4( data[lowest], data[lowest + length * 3 / 4 - 1], data[lowest + length * 7 / 8 - 1], data[size - 1] );
    return ranges;
}
 
int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
 
    GLFWwindow * window = glfwCreateWindow(screen_width, screen_height, "Mandelbrot_CED18I039", NULL, NULL);
 
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window!\n";
        glfwTerminate();
        return -1;
    }
 
    glfwMakeContextCurrent(window);
 
    if (glewInit())
    {
        std::cout << "Failed initializing GLEW\n";
    }
 
    glViewport(0, 0, screen_width, screen_height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
     
     // call the shaders
    Shader our_shader("Shaders/shader.vert", "Shaders/shader.frag");
 
 //refer to different VAOs to draw the corresponding vertices.
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
 
    glBindVertexArray(VAO);
 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
 
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
 
    last_time = glfwGetTime();
 
    glEnable(GL_DEPTH_TEST);
    //set up the initial ranges.
    std::vector<float> pixel_data(screen_width * screen_height, 0.0f);
    glm::vec4 ranges = glm::vec4(0.0001f, 0.33333f, 0.66667f, 1.00f);
 
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.0f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
        process_input(window);
        countFPS();

 // send the values of center_x, center_y, and zoom to their appropriately named uniform counterparts in the fragment shader.
        our_shader.use_shader();
        our_shader.set_float("zoom", zoom);
        our_shader.set_float("center_x", center_x);
        our_shader.set_float("center_y", center_y);
        our_shader.set_vec4("color_ranges", ranges);
 
        glBindVertexArray(VAO);
         
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
 
        glfwSwapBuffers(window);
        glfwPollEvents();
        // iteration values for each fragment using glReadPixels, and then pass them to our find_ranges function.
        glReadPixels(0, 0, screen_width, screen_height, GL_DEPTH_COMPONENT, GL_FLOAT, pixel_data.data());
        ranges = find_ranges(pixel_data);
    }
 
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
 
    glfwTerminate();
    return 0;
}
