#version 330 core

uniform float width;
uniform float height;

out vec4 FragColor;

void main()
{
    vec2 st = vec2(gl_FragCoord.x / 640, gl_FragCoord.y / 480);
    gl_FragColor = vec4(st.x, st.y, 0.0, 1.0);
}