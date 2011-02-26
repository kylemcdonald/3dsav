
varying vec4 position;

uniform float focalPoint;

void main() {
	
	gl_Position =  gl_ModelViewProjectionMatrix * gl_Vertex;
	
	float size =  1.0 - clamp((abs(gl_Position.z - focalPoint) / 400.0), 0.0, 1.0);
	
	gl_FrontColor = gl_Color;
	gl_FrontColor.a = size;
	
	
}
