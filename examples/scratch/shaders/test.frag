#version 450 core

in vec2 TexCoord;

layout(location = 0) out vec4 FragColor;

uniform float testValue;
uniform sampler2D testTexture;

void main()
{
    FragColor = vec4(texture(testTexture, vec2(TexCoord.x, TexCoord.y + testValue*sin(20*TexCoord.x))).rgb, 1.0);
    // FragColor = vec4(0.8, 0.5, testValue, 1.0);
}