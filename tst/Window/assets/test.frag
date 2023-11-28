#version 330 core

uniform int myUniformInt;

out vec4 FragColor;

void main()
{
    vec2 st = vec2(gl_FragCoord.x * myUniformInt, gl_FragCoord.y / 480);
    gl_FragColor = vec4(st.x, st.y, 0.0, 1.0);
}