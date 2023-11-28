#version 330 core

uniform int u_width;
uniform int u_height;

out vec4 FragColor;

void main()
{
    vec2 st = vec2(gl_FragCoord.x / u_width, gl_FragCoord.y / u_height);
    gl_FragColor = vec4(st.x, st.y, 0.0, 1.0);
}