#version 450 core

out vec4 FragColor;

uniform float patternSize = 50.;

uniform vec4 highColor = vec4(0.5, 0.0, 0.5, 1.0);
uniform vec4 lowColor = vec4(0.5, 0.0, 0.0, 0.0);

void main()
{

    FragColor = vec4(1.0, 1.0, 0.0, 1.0);
}
