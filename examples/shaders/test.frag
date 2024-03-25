#version 450 core

layout(location = 0) out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D testTexture;

void main()
{
    FragColor = vec4(texture(testTexture, TexCoord).xyz, 1.0);
}
