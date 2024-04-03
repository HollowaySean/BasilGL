#version 450 core

layout(location = 0) out vec4 FragColor;

in vec2 TexCoord;

uniform float testValue;

uniform sampler2D testTexture;

void main()
{
    FragColor = vec4(texture(testTexture, TexCoord / testValue).xyz, 1.0);
}
