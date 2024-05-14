#version 450 core

out vec4 fragColor;

uniform float testValue;
uniform vec2 iResolution = vec2(400.0, 400.0);
uniform vec4 iMouse;

void main()
{
    vec3 col = vec3(0.);

    //Draw a red cross where the mouse button was last down.
    if(abs(iMouse.x-gl_FragCoord.x) < 4.) {
        col = vec3(1.,0.,0.);
    }
    if(abs(iMouse.y-gl_FragCoord.y) < 4.) {
        col = vec3(1.,0.,0.);
    }

    //If the button is currently up, (iMouse.z, iMouse.w) is where the mouse
    //was when the button last went down.
    if(abs(iMouse.z-gl_FragCoord.x) < 2.) {
        col = vec3(0.,0.,1.);
    }
    if(abs(iMouse.w-gl_FragCoord.y) < 2.) {
        col = vec3(0.,0.,1.);
    }

    //If the button is currently down, (-iMouse.z, -iMouse.w) is where
    //the button was when the click occurred.
    if(abs(-iMouse.z-gl_FragCoord.x) < 2.) {
        col = vec3(0.,1.,0.);
    }
    if(abs(-iMouse.w-gl_FragCoord.y) < 2.) {
        col = vec3(0.,1.,0.);
    }

    fragColor = vec4(col, 1.0);
}