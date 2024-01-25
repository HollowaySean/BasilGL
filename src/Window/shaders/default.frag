#version 430 core

out vec4 FragColor;

uniform float patternSize = 50.;

uniform vec4 highColor = vec4(1.0, 0.0, 1.0, 1.0);
uniform vec4 lowColor = vec4(0.0, 0.0, 0.0, 0.0);

void main()
{
    vec2 coord = floor(gl_FragCoord.xy / patternSize);
    float mask = mod(coord.x + mod(coord.y, 2.0), 2.0);

    FragColor = mask * highColor;
}
