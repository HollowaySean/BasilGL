#version 450 core

in vec2 TexCoord;

layout(location = 0) out vec4 FragColor;

uniform float maximumValue;
uniform float textureVisibility;
uniform float iTime;

uniform sampler2D testTexture;

void main()
{
    vec4 textureValue = texture(testTexture, TexCoord);
    float currentVisibility = textureVisibility * (1 - 0.5*sin(iTime));

    textureValue = currentVisibility * vec4(textureValue.rgb, 0.0);

    FragColor =
        vec4(
            clamp(TexCoord.x, 0, maximumValue),
            clamp(TexCoord.y, 0, maximumValue),
            0.0, 1.0)
        + textureValue;
}