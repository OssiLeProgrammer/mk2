#version 300 es
layout(location = 0) in highp vec3 aPos;

uniform highp mat4 cMatrix;
uniform highp mat4 wMatrix;

void main()
{
    gl_Position = cMatrix * wMatrix * vec4(aPos, 1.0);
}