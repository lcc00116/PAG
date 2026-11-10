#version 410
out vec4 colorFragmento;

// Color difuso del material del modelo que se está dibujando
uniform vec3 uColorDifuso;

subroutine vec4 fCalcularColor();
subroutine uniform fCalcularColor uMetodoColorElegido;

// Modo alambre: color fijo (rojo)
subroutine ( fCalcularColor )
vec4 colorAlambre ()
{ return vec4 ( 1, 0, 0, 1 ); }

// Modo solido: el color difuso del material (el último 1 es la opacidad)
subroutine ( fCalcularColor )
vec4 colorSolido ()
{ return vec4 ( uColorDifuso, 1 ); }

void main ()
{ colorFragmento = uMetodoColorElegido (); }