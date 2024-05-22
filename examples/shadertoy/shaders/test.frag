#version 450 core

in vec2 TexCoord;

layout(location = 0) out vec4 FragColor;

uniform float maximumValue;
uniform float textureVisibility;

uniform sampler2D testTexture;

void main()
{
    vec4 textureValue = texture(testTexture, TexCoord);

    FragColor =
        vec4(
            clamp(TexCoord.x, 0, maximumValue),
            clamp(TexCoord.y, 0, maximumValue),
            0.0, 1.0)
        + textureVisibility * vec4(
            textureValue.rgb, 0.0);
}