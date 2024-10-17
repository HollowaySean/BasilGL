#version 330 core
#pragma optimize (off)

uniform bool    myBool1;
uniform int     myInt1;
uniform uint    myUnsignedInt1;
uniform float   myFloat1;

uniform bvec2   myBool2;
uniform ivec2   myInt2;
uniform uvec2   myUnsignedInt2;
uniform vec2    myFloat2;

uniform bvec3   myBool3;
uniform ivec3   myInt3;
uniform uvec3   myUnsignedInt3;
uniform vec3    myFloat3;

uniform bvec4   myBool4;
uniform ivec4   myInt4;
uniform uvec4   myUnsignedInt4;
uniform vec4    myFloat4;

uniform mat2x2 myFloat2x2;
uniform mat2x3 myFloat2x3;
uniform mat2x4 myFloat2x4;
uniform mat3x3 myFloat3x3;
uniform mat3x4 myFloat3x4;
uniform mat4x4 myFloat4x4;

uniform sampler2D testTex;

out vec4 FragColor;

void main()
{
    // Dummy code used to prevent compiler from optimizing out
    //  uniforms under test
    float testVal
        = myFloat2x2[0][0] + myFloat2x3[0][0] + myFloat2x4[0][0] +
          myFloat3x3[0][0] + myFloat3x4[0][0] + myFloat4x4[0][0];
    testVal += (myBool1   ? myInt1 : int(myUnsignedInt1))   * myFloat1;
    vec2 testVal2 = (myBool2.y ? myInt2 : ivec2(myUnsignedInt2)) * myFloat2;
    vec3 testVal3 = (myBool3.z ? myInt3 : ivec3(myUnsignedInt3)) * myFloat3;
    vec4 testVal4 = (myBool4.w ? myInt4 : ivec4(myUnsignedInt4)) * myFloat4;

    vec2 st = vec2(gl_FragCoord.x * myInt1, gl_FragCoord.y / 480);
    vec4 outColor = vec4(st.x, st.y, testVal + testVal2.x + testVal3.y + testVal4.z, 1.0);
    vec4 texColor = texture(testTex, gl_FragCoord.xy) * vec4(1.0, 1.0, 1.0, 1.0);

    gl_FragColor = mix(texColor, outColor, 0.5);
}