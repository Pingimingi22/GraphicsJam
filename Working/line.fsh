#version 450 core
out vec4 FragColor;

uniform vec3 lineColour;

void main()
{
    FragColor = vec4(lineColour.x, lineColour.y, lineColour.z, 1.0);
} 