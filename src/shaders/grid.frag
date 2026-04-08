#version 330 core
out vec4 FragColor;

uniform float WINDOW_WIDTH;
uniform float WINDOW_HEIGHT;

void main() {
    float grid_size = 20;
    vec2 uv = gl_FragCoord.xy / vec2(WINDOW_WIDTH, WINDOW_HEIGHT); //normalized cords (0.0 to 1.0)
    vec2 grid_pos = fract(uv * grid_size); //returns the decimal part (fract(x) = x - floor(x))

    //step() creates a threshold, in this case 0.99, and returns 1.0 if it passes, 0.0 if it doesn't
    vec2 sworm = fwidth(uv * grid_size);
    vec2 lines = smoothstep(1.0 - sworm, vec2(1.0), grid_pos);

    float line = max(lines.x, lines.y);
    
    float axisX = step(0.499, uv.x) * step(uv.x, 0.501); //multiplies by zero if it's not the exact middle
    float axisY = step(0.499, uv.y) * step(uv.y, 0.501);
    
    vec4 gridColor = vec4(vec3(0.5), 1.0); // grid
    vec4 backgroundColor = vec4(0.0, 0.0, 0.0, 0.0);

    if (axisX > 0.0 || axisY > 0.0) gridColor = vec4(vec3(0.75), 1.0); //axis

    FragColor = mix(backgroundColor, gridColor, line);
}