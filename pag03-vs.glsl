#version 410
layout (location = 0) in vec3 posicion;
layout (location = 1) in vec3 color;

uniform mat4 mModelViewProj;

out vec3 colorInterpolado;

void main ()
{
    colorInterpolado = color;
    gl_Position = mModelViewProj * vec4 ( posicion, 1 );
}