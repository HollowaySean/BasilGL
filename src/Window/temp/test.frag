#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D testTexture;

uniform float bMod = 1.0;

void main()
{
    // vec2 st = vec2(TexCoord.x, TexCoord.y);
    // gl_FragColor = vec4(st.x, st.y, 0.0, 1.0);
    float texPoint = bMod * texture(testTexture, TexCoord).x;
    FragColor = vec4(texPoint, texPoint, texPoint, 1.0);
}
