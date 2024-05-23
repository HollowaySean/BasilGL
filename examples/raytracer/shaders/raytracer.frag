#version 450 core

in vec2 TexCoord;

layout(location = 0) out vec4 FragColor;

void main()
{
    FragColor = vec4(0.5, 0.1, 0.0, 1.0);
}