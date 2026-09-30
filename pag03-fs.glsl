#version 410
in vec3 colorInterpolado;
out vec4 colorFragmento;

void main ()
{
    colorFragmento = vec4 ( colorInterpolado, 1.0 );
}