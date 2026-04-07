#version 330 core
out vec4 FragColor;

int counter = 0;

void main() {

    //static red line for the graph
    //FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);

    // example of a little shader that create a rainbow effect based on screen position
    float x = gl_FragCoord.x * 0.01; 
    
    float r = sin(x) * 0.5 + 0.5;
    float g = sin(x + 2.0) * 0.5 + 0.5;
    float b = sin(x + 4.0) * 0.5 + 0.5;

    FragColor = vec4(r, g, b, 1.0f);
}