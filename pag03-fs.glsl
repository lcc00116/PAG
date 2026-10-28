#version 410
out vec4 colorFragmento;

subroutine vec4 fCalcularColor();
subroutine uniform fCalcularColor uMetodoColorElegido;

subroutine ( fCalcularColor )
vec4 colorAlambre ()
{ return vec4 ( 1, 0, 0, 1 ); }

subroutine ( fCalcularColor )
vec4 colorSolido ()
{ return vec4 ( 0, 1, 0, 1 ); }

void main ()
{ colorFragmento = uMetodoColorElegido (); }