#version 330 core
#pragma optimize (off)

uniform bool    myUniformBool;
uniform int     myUniformInt;
uniform uint    myUniformUnsignedInt;
uniform float   myUniformFloat;

uniform bvec2   myUniformBool2;
uniform ivec2   myUniformInt2;
uniform uvec2   myUniformUnsignedInt2;
uniform vec2    myUniformFloat2;

uniform bvec3   myUniformBool3;
uniform ivec3   myUniformInt3;
uniform uvec3   myUniformUnsignedInt3;
uniform vec3    myUniformFloat3;

uniform bvec4   myUniformBool4;
uniform ivec4   myUniformInt4;
uniform uvec4   myUniformUnsignedInt4;
uniform vec4    myUniformFloat4;

uniform sampler2D testTex;

out vec4 FragColor;

void main()
{
    // Dummy code used to prevent compiler from optimizing out
    //  uniforms under test
    float testVal = (myUniformBool ? myUniformInt : int(myUniformUnsignedInt))
        * myUniformFloat;
    vec2 testVal2 = (myUniformBool2.y ? myUniformInt2 : ivec2(myUniformUnsignedInt2))
        * myUniformFloat2;
    vec3 testVal3 = (myUniformBool3.z ? myUniformInt3 : ivec3(myUniformUnsignedInt3))
        * myUniformFloat3;
    vec4 testVal4 = (myUniformBool4.w ? myUniformInt4 : ivec4(myUniformUnsignedInt4))
        * myUniformFloat4;

    vec2 st = vec2(gl_FragCoord.x * myUniformInt, gl_FragCoord.y / 480);
    vec4 outColor = vec4(st.x, st.y, testVal + testVal2.x + testVal3.y + testVal4.z, 1.0);
    vec4 texColor = texture(testTex, gl_FragCoord.xy) * vec4(1.0, 1.0, 1.0, 1.0);

    gl_FragColor = mix(texColor, outColor, 0.5);
}