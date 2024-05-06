#version 450 core

out vec4 FragColor;

uniform float testValue;

void main()
{
    FragColor = vec4(0.8, 0.5, testValue, 1.0);
}
