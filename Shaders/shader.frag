#version 330 core
// The fragment shader starts with telling the GPU that we want to take the position of the fragment as an input, and output the final color value.
in vec4 gl_FragCoord;
 // fragments represent the actual pixels
out vec4 fragColor;
 
uniform float center_x;
uniform float center_y;
uniform float zoom;
uniform vec4 color_ranges;
 
#define MAX_ITERATIONS 600
 
 //position of the fragment in the rendered view as the coordinates in complex space that is passed to Mandelbrot algorithm.
int Compute_iterations_RI()
{
    // position of the fragment in gl_FragCoord through integers starting from 0 to the width and height of the rendered view.
    // divide these coordinates by the width and the height to get a real and a complex number, respectively, between 0 and 1. 
    float real = ((gl_FragCoord.x / 2800.0f - 0.5f) * zoom + center_x) * 4.0f; // lower than 1080 will cause imbalance , 2046 , 2800 for waaaaayyy too deep.
    float imag = ((gl_FragCoord.y / 2800.0f - 0.5f) * zoom + center_y) * 4.0f;
    //  shift them a bit to center them more or less around the origin, and then “zoom in” by scaling by a factor of four.
    int iterations = 0;
    float const_real = real;
    float const_imag = imag;
 
    while (iterations < MAX_ITERATIONS)
    {
        float tmp_real = real;
        real = (real * real - imag * imag) + const_real;
        imag = (2.0 * tmp_real * imag) + const_imag;
         
        float dist = real * real + imag * imag;
 
        if (dist > 4.0)
            break;
 
        ++iterations;
    }
    return iterations;
}
 
 // fragmant color function ( reffered from blogs and cherno videos )

 // count of the iterations to color the fragment via the return_color() function. 
 // If the iteration count is MAX_ITERATIONS we color the fragment black.
vec4 return_color()
{
    int iter = Compute_iterations_RI();
    if (iter == MAX_ITERATIONS)
    {
        gl_FragDepth = 0.0f;
        return vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
 
    float iterations = float(iter) / MAX_ITERATIONS;
    // encode the information about the number of iterations in the depth buffer. 
    // depth buffer array of floating point values between 0 and 1 representing the distance of each fragment from the viewpoint.
    gl_FragDepth = iterations;
    // colors used in shader for mixing .
                                                                        // r + g = y, r + b = m, b + g = c
    vec4 color_0 = vec4(0.0f, 0.0f, 0.0f, 1.0f); // r, g, b , background
    vec4 color_1 = vec4(0.0f, 0.8f, 0.0f, 1.0f); // outer 0.0f, 0.8f, 0.0f, 1.0f
    vec4 color_2 = vec4(1.0f, 0.5f, 0.0f, 1.0f); // middle
    vec4 color_3 = vec4(1.0f, 0.0f, 0.4f, 1.0f); // mixer over all
 
 //intensity of the color being directly proportional to the number of iterations of the algorithm took to discard that point.
/*
    lowest and the highest number of iterations corresponding to the rendered view 
    build a histogram of all values between the bounds, and color each bin, or range, in different colors. 
    Preserves the same contrast at any zoom level. 
*/

// calculate the appropriate iteration ranges for the various colors.
    float fraction = 0.0f;
    if (iterations < color_ranges[1])
    {
        // take the relative number of iterations and look in which range it falls. 
        // For instance, if it’s between the first and second value of the vector color_ranges, we color the fragment by mixing color_0 and color_1 
        fraction = (iterations - color_ranges[0]) / (color_ranges[1] - color_ranges[0]);
        return mix(color_0, color_1, fraction);
    }
    else if(iterations < color_ranges[2])
    {
        fraction = (iterations - color_ranges[1]) / (color_ranges[2] - color_ranges[1]);
        return mix(color_1, color_2, fraction);
    }
    else
    {
        fraction = (iterations - color_ranges[2]) / (color_ranges[3] - color_ranges[2]);
        return mix(color_2, color_3, fraction);
    }
}
 
void main()
{
    fragColor = return_color();
}